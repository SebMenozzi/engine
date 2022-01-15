#include "terrain.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb_image.h>

namespace object
{
    Terrain::Terrain(
        const char* filepath, 
        float scale, 
        float minHeight, 
        float maxHeight
    ): 
        Heightmap(0),
        scale_(scale),
        minHeight_(minHeight),
        maxHeight_(maxHeight)
    {
        int width, height, channels;
        uint8* data = stbi_load(filepath, &width, &height, &channels, 3);

        if (!data)
        {
            std::cerr << "Failure to load the image: " << filepath << std::endl;
            return;
        }

        int totalBytes = width * height * channels;
        int i = 1;

        std::vector<float> tmp;
        for (uint8 *p = data; p != data + totalBytes; p += channels)
        {
            float average = (*p + *(p + 1) + *(p + 2)) / 3;
            float scaled = utils::scale(average, 0.0, 255.0, minHeight_, maxHeight_);
            tmp.push_back(scaled);

            if (i % width == 0)
            {
                heights_.push_back(tmp);
                tmp.clear();
            }

            ++i;
        }

        size_ = height;

        auto vertices = new std::vector<glm::vec3>();
        auto normals = new std::vector<glm::vec3>();

        for (size_t x = 0; x < size_; ++x)
            for (size_t z = 0; z < size_; ++z)
                addVertices_(vertices, x, z);

        for (size_t i = 0; i < vertices->size(); ++i)
        {
            auto n = computeNormal_((*vertices)[i].x, (*vertices)[i].z);
            normals->push_back(n);
        }

        vertices_ = Buffer { reinterpret_cast<const uint8*>(vertices->data()), sizeof(glm::vec3), vertices->size() };
        normals_ = Buffer { reinterpret_cast<const uint8*>(normals->data()), sizeof(glm::vec3), normals->size() };
    }

    void Terrain::addVertices_(std::vector<glm::vec3>* vertices, float x, float z)
    {
        float h0 = getHeight(x, z);
        float h1 = getHeight(x + 1, z);
        float h2 = getHeight(x, z + 1);
        float h3 = getHeight(x + 1, z + 1);

        // 0
        vertices->push_back(
            glm::vec3(x * scale_, h0, z * scale_)
        );

        // 1 
        vertices->push_back(
            glm::vec3((x + 1) * scale_, h1, z * scale_)
        );

        // 2 
        vertices->push_back(
            glm::vec3(x * scale_, h2, (z + 1) * scale_)
        );

        // 2
        vertices->push_back(
            glm::vec3(x * scale_, h2, (z + 1) * scale_)
        );

        // 3
        vertices->push_back(
            glm::vec3((x + 1) * scale_, h3, (z + 1) * scale_)
        );

        // 1 
        vertices->push_back(
            glm::vec3((x + 1) * scale_, h1, z * scale_)
        );
    }

    glm::vec3 Terrain::computeNormal_(float x, float z)
    {
        float hl = getHeight(x - 1, z);
        float hr = getHeight(x + 1, z);
        float hd = getHeight(x, z + 1);
        float hu = getHeight(x, z - 1);

        glm::vec3 n = glm::vec3(hl - hr, 1.0f, hd - hu);

        return glm::normalize(n);
    }
}