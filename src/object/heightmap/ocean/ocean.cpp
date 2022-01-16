#include "ocean.h"

#include <gtc/noise.hpp>

namespace object
{
    float Ocean::computePerlinHeight(int x, int z, float time)
    {
        // perlin wave paremeters
        const float perlinGlobalFundFreq = 3.f;
        const float perlinXFundFreq = 2.f;
        const float perlinZFundFreq = 2.f;
        const float perlinAmplitude = 0.5f;
        const float perlinGlobalAmplitude = 0.1f;
        const float perlinTimeScale = 1 / 2.5f;
        const int perlinNbOctaves = 5;

        float perlinHeight = 0;

        float xFreq = perlinGlobalFundFreq * perlinXFundFreq / size_;
        float zFreq = perlinGlobalFundFreq * perlinZFundFreq / size_;
        float a = perlinAmplitude;

        for (int oct = 0; oct < perlinNbOctaves; oct++)
        {
            glm::vec3 p(x * xFreq, z * zFreq, time * perlinTimeScale);
            perlinHeight += glm::perlin(p) * a;

            xFreq *= perlinXFundFreq;
            zFreq *= perlinZFundFreq;
            a *= perlinAmplitude;
        }

        return perlinHeight * perlinGlobalAmplitude;
    }

    float computeSinusoidalHeight(float x, float z, uint32 time)
    {
        // simple sinusoidal wave parameters
        const float a = 0.015f;
        const float kx = 160.0f;
        const float kz = 200.0f;
        const float w = 0.001f;

        return a * std::sin(kx * x + kz * z + w * time);
    }

    Ocean::Ocean(
        float size,
        float scale
    ):
        Heightmap(size),
        scale_(scale)
    {
        auto vertices = new std::vector<glm::vec3>();
        auto normals = new std::vector<glm::vec3>();

        for (size_t x = 0; x < size_; ++x)
        {
            for (size_t z = 0; z < size_; ++z)
            {
                // 0
                vertices->push_back(glm::vec3(x * scale_, 0, z * scale_));
                // 1 
                vertices->push_back(glm::vec3((x + 1) * scale_, 0, z * scale_));
                // 2 
                vertices->push_back(glm::vec3(x * scale_, 0, (z + 1) * scale_));
                // 2
                vertices->push_back(glm::vec3(x * scale_, 0, (z + 1) * scale_));
                // 3
                vertices->push_back(glm::vec3((x + 1) * scale_, 0, (z + 1) * scale_));
                // 1 
                vertices->push_back(glm::vec3((x + 1) * scale_, 0, z * scale_));
            }
        }

        for (size_t i = 0; i < vertices->size(); ++i)
            normals->push_back(glm::vec3(0.0, 1.0, 0.0));

        vertices_ = Buffer { reinterpret_cast<const uint8*>(vertices->data()), sizeof(glm::vec3), vertices->size() };
        normals_ = Buffer { reinterpret_cast<const uint8*>(normals->data()), sizeof(glm::vec3), normals->size() };

        for (int x = 0; x < size_ + 1; ++x)
        {
            std::vector<std::array<float, perlinLoopFrameLength_>> col;
            for (int z = 0; z < size_ + 1; ++z)
            {
                std::array<float, perlinLoopFrameLength_> frameHeights;
                for (int i = 0; i < perlinLoopFrameLength_; i++)
                    frameHeights[i] = computePerlinHeight(x, z, i / (float) perlinLoopFps_);

                col.push_back(frameHeights);
            }

            perlinLoopFrames_.push_back(col);
        }
    }

    float Ocean::interpolatePerlinHeight(int x, int z, uint32 timeMs)
    {
        // convert time in seconds
        float time = timeMs / 1000.f;

        int frameI1, frameI2;

        if ((int) (time / (perlinLoopDuration_ - perlinLoopPeriod)) % 2 == 0)
        {
            // frontward play
            time = std::fmod(time, perlinLoopDuration_ - perlinLoopPeriod);

            frameI1 = time * perlinLoopFps_;
            frameI2 = frameI1 + 1;
        }
        else
        {
            // backward play
            time = perlinLoopDuration_ - perlinLoopPeriod - std::fmod(time, perlinLoopDuration_ - perlinLoopPeriod);

            frameI2 = std::ceil(time * perlinLoopFps_);
            frameI1 = frameI2 - 1;
        }

        auto frameHeights = perlinLoopFrames_[x][z];

        float time1 = frameI1 / (float) perlinLoopFps_;
        float time2 = frameI2 / (float) perlinLoopFps_;

        float t = (time2 - time) / (time2 - time1);

        return t * frameHeights[frameI1] + (1 - t) * frameHeights[frameI2];
    }

    void Ocean::updateHeights(uint32 time)
    {
        const uint32 totalSize = vertices_.dataStride_ * vertices_.nbElements_;

        uint8* data = const_cast<uint8*>(vertices_.data_);

        for (uint8* p = data; p != data + totalSize; p += vertices_.dataStride_)
        {
            glm::vec3* pVec3 = reinterpret_cast<glm::vec3*>(p);
            glm::vec3 e = *pVec3;

            int x = e.x / scale_;
            int z = e.z / scale_;

            float newHeight = interpolatePerlinHeight(x, z, time);
            *pVec3 = glm::vec3(e.x, newHeight, e.z);
        }
    }

    glm::vec3 Ocean::computeNormal(int x, int z)
    {
        if (x >= size_ || z >= size_)
            return glm::vec3(0, 1, 0);

        float strength = 64.f;  // NOTE I'm not sure how to figure out this constant

        uint8* verticesData = const_cast<uint8*>(vertices_.data_);
        glm::vec3* verticesVec3 = reinterpret_cast<glm::vec3*>(verticesData);

        // verticesVec3[(int) (x * size_ + z) * 6]; is the vertex at position (x, z)
        const glm::vec3 pRight = verticesVec3[(int) ((x + (x < size_ - 1)) * size_ + z) * 6];
        const glm::vec3 pLeft = verticesVec3[(int) ((x - (x != 0)) * size_ + z) * 6];
        const glm::vec3 pUp = verticesVec3[(int) (x * size_ + (z + (z < size_ - 1))) * 6];
        const glm::vec3 pDown = verticesVec3[(int) (x * size_ + (z - (z != 0))) * 6];

        // central difference : f(x + 1) - f(x - 1) ?
        float nX = pRight.y - pLeft.y;
        float nY = 1 / strength;
        float nZ = pUp.y - pDown.y;

        return glm::normalize(glm::vec3(nX, nY, nZ));
    }

    void Ocean::updateNormals()
    {
        const uint32 totalSize = normals_.dataStride_ * normals_.nbElements_;

        uint8* normalsData = const_cast<uint8*>(normals_.data_);
        uint8* verticesData = const_cast<uint8*>(vertices_.data_);

        uint8* n = normalsData;
        uint8* p = verticesData;

        while (n != normalsData + totalSize)
        {
            glm::vec3* nVec3 = reinterpret_cast<glm::vec3*>(n);

            // figure out current vertex coordinates
            glm::vec3 e = *reinterpret_cast<glm::vec3*>(p);
            int x = e.x / scale_, z = e.z / scale_;

            nVec3[0] = computeNormal(x, z);
            nVec3[1] = computeNormal(x + 1, z);
            nVec3[2] = computeNormal(x, z + 1);
            nVec3[3] = computeNormal(x, z + 1);
            nVec3[4] = computeNormal(x + 1, z + 1);
            nVec3[5] = computeNormal(x + 1, z);

            n += normals_.dataStride_ * 6;
            p += vertices_.dataStride_ * 6;
        }
    }
}
