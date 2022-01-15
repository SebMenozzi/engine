#include "ocean.h"

namespace object
{
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
    }

    void Ocean::updateHeights(uint32 time)
    {
        const float a = 0.015f;
        const float kx = 160.0f;
        const float kz = 200.0f;
        const float w = 0.001f;

        const uint32 totalSize = vertices_.dataStride_ * vertices_.nbElements_;
        
        uint32 i = 0;

        uint8* data = const_cast<uint8*>(vertices_.data_);

        for (uint8* p = data; p != data + totalSize; p += vertices_.dataStride_)
        {
            glm::vec3* pVec3 = reinterpret_cast<glm::vec3*>(p);
            glm::vec3 e = *pVec3;

            float newHeight = a * std::sin(kx * e.x + kz * e.z + w * time);
            *pVec3 = glm::vec3(e.x, newHeight, e.z);

            ++i;
        }
    }
}
