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

        for (int x = 0; x < size_; ++x)
        {
            std::vector<std::array<float, perlinLoopFrameLength_>> col;
            for (int z = 0; z < size_; ++z)
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

        // FIXME We should determine those values instead of hardcoding them
        float minFloatX = -1;
        float minFloatZ = -1;
        float maxFloatX = 3.2;
        float maxFloatZ = 3.2;

        for (uint8* p = data; p != data + totalSize; p += vertices_.dataStride_)
        {
            glm::vec3* pVec3 = reinterpret_cast<glm::vec3*>(p);
            glm::vec3 e = *pVec3;

            int x = utils::scale(e.x, minFloatX, maxFloatX, 0, size_ - 1);
            int z = utils::scale(e.z, minFloatZ, maxFloatZ, 0, size_ - 1);

            float newHeight = interpolatePerlinHeight(x, z, time);
            *pVec3 = glm::vec3(e.x, newHeight, e.z);
        }
    }
}
