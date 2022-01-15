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
        std::vector<float> tmp;
        for (int x = 0; x < size_; ++x)
        {
            for (int z = 0; z < size_; ++z)
                tmp.push_back(height);
            heights_.push_back(tmp);
            tmp.clear();
        }

        for (size_t x = 0; x < size_; ++x)
        {
            for (size_t z = 0; z < size_; ++z)
            {
                // 0
                glmVertices_.push_back(glm::vec3(x, height_, z));
                // 1 
                glmVertices_.push_back(glm::vec3(x + 1, height_, z));
                // 2 
                glmVertices_.push_back(glm::vec3(x, height_, z + 1));
                // 2
                glmVertices_.push_back(glm::vec3(x, height_, z + 1));
                // 3
                glmVertices_.push_back(glm::vec3(x + 1, height_, z + 1));
                // 1 
                glmVertices_.push_back(glm::vec3(x + 1, height_, z));
            }
        }

        for (size_t i = 0; i < glmVertices_.size(); ++i)
            glmNormals_.push_back(glm::vec3(0.0, 1.0, 0.0));

        vertices_ = Buffer { reinterpret_cast<const uint8*>(glmVertices_.data()), sizeof(glm::vec3), glmVertices_.size() };
        normals_ = Buffer { reinterpret_cast<const uint8*>(glmNormals_.data()), sizeof(glm::vec3), glmNormals_.size() };
    }

    void Ocean::updateHeights(float t)
    {
        // simple sinusoidal wave formula
        // a * sin(k_x * x + k_z * z + w * t)
        const float a = 0.5f;
        const float k_x = 1 / size_, k_z = 1 / size_;
        const float w = 1.f;

        for (size_t tileX = 0; tileX < size_; tileX++)
            for (size_t tileZ = 0; tileZ < size_; tileZ++)
                heights_[tileX][tileZ] = height_ + a * std::sin(k_x * tileX + k_z * tileZ + w * t);
    }

    // FIXME only updates vertices for now
    void Ocean::updateVerticesAndNormals()
    {
        constexpr size_t nb_vertice_per_tile = 6;

        for (size_t i = 0; i < glmVertices_.size() / nb_vertice_per_tile; i++)
        {
            int tileX = i / (int) size_, tileZ = i % (int) size_;
            auto tileHeight = getTileHeight_(tileX, tileZ);

            size_t v_i_start = i * nb_vertice_per_tile;

            for (size_t j = 0; j < nb_vertice_per_tile; j++)
                glmVertices_[v_i_start + j].y = tileHeight;
        }

        vertices_ = Buffer { reinterpret_cast<const uint8*>(glmVertices_.data()), sizeof(glm::vec3), glmVertices_.size() };
        normals_ = Buffer { reinterpret_cast<const uint8*>(glmNormals_.data()), sizeof(glm::vec3), glmNormals_.size() };
    }
}
