#include "ocean.h"

namespace object
{
    Ocean::Ocean(
        float size,
        float height
    ): 
        Heightmap(size),
        height_(height)
    {
        auto vertices = new std::vector<glm::vec3>();
        auto normals = new std::vector<glm::vec3>();

        for (size_t x = 0; x < size_; ++x)
        {
            for (size_t z = 0; z < size_; ++z)
            {
                // 0
                vertices->push_back(glm::vec3(x, height_, z));
                // 1 
                vertices->push_back(glm::vec3(x + 1, height_, z));
                // 2 
                vertices->push_back(glm::vec3(x, height_, z + 1));
                // 2
                vertices->push_back(glm::vec3(x, height_, z + 1));
                // 3
                vertices->push_back(glm::vec3(x + 1, height_, z + 1));
                // 1 
                vertices->push_back(glm::vec3(x + 1, height_, z));
            }
        }

        for (size_t i = 0; i < vertices->size(); ++i)
            normals->push_back(glm::vec3(0.0, 1.0, 0.0));

        vertices_ = Buffer { reinterpret_cast<const uint8*>(vertices->data()), sizeof(glm::vec3), vertices->size() };
        normals_ = Buffer { reinterpret_cast<const uint8*>(normals->data()), sizeof(glm::vec3), normals->size() };
    }
}
