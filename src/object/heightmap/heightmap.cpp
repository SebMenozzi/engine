#include "heightmap.h"

namespace object
{
    Heightmap::Heightmap(uint32 size): 
        Mesh(),
        size_(size)
    {
        auto vertices = new std::vector<glm::vec3>();
        auto normals = new std::vector<glm::vec3>();
        auto uvs = new std::vector<glm::vec2>();

        for (size_t x = 0; x < size_; ++x)
        {
            for (size_t z = 0; z < size_; ++z)
            {
                // 2 
                vertices->push_back(glm::vec3(x, 0, z + 1));
                normals->push_back(glm::vec3(0.0, 1.0, 0.0));
                uvs->push_back(glm::vec2(0.0f, 1.0f));
                // 3
                vertices->push_back(glm::vec3(x + 1, 0, z + 1));
                normals->push_back(glm::vec3(0.0, 1.0, 0.0));
                uvs->push_back(glm::vec2(1.0f, 1.0f));
                // 1 
                vertices->push_back(glm::vec3(x + 1, 0, z));
                normals->push_back(glm::vec3(0.0, 1.0, 0.0));
                uvs->push_back(glm::vec2(1.0f, 0.0f));
                // 1 
                vertices->push_back(glm::vec3(x + 1, 0, z));
                normals->push_back(glm::vec3(0.0, 1.0, 0.0));
                uvs->push_back(glm::vec2(1.0f, 0.0f));
                // 0
                vertices->push_back(glm::vec3(x, 0, z));
                normals->push_back(glm::vec3(0.0, 1.0, 0.0));
                uvs->push_back(glm::vec2(0.0f, 0.0f));
                // 2 
                vertices->push_back(glm::vec3(x, 0, z + 1));
                normals->push_back(glm::vec3(0.0, 1.0, 0.0));
                uvs->push_back(glm::vec2(0.0f, 1.0f));
            }
        }

        vertices_ = Buffer { reinterpret_cast<const uint8*>(vertices->data()), sizeof(glm::vec3), vertices->size() };
        normals_ = Buffer { reinterpret_cast<const uint8*>(normals->data()), sizeof(glm::vec3), normals->size() };
        uvs_ = Buffer { reinterpret_cast<const uint8*>(uvs->data()), sizeof(glm::vec2), uvs->size() };
    }
}
