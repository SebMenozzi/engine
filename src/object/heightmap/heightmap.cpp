#include "heightmap.h"

namespace object
{
    Heightmap::Heightmap(
        float size
    ): 
        Mesh(),
        size_(size)
    {}

    float Heightmap::getHeight(float x, float z)
    {
        glm::vec2 current(x, z);

        glm::vec2 neighbor1(std::floor(x), std::floor(z));
        glm::vec2 neighbor2(std::ceil(x), std::floor(z));
        glm::vec2 neighbor3(std::floor(x), std::ceil(z));
        glm::vec2 neighbor4(std::ceil(x), std::ceil(z));
        
        float neighbor1Height = getTileHeight_(neighbor1.x, neighbor1.y);
        float neighbor2Height = getTileHeight_(neighbor2.x, neighbor2.y);
        float neighbor3Height = getTileHeight_(neighbor3.x, neighbor3.y);
        float neighbor4Height = getTileHeight_(neighbor4.x, neighbor4.y);

        // https://math.stackexchange.com/questions/3230376/interpolate-between-4-points-on-a-2d-plane
        float c1 = (1 - x) * (1 - z) * neighbor1Height;
        float c2 = x * (1 - z) * neighbor2Height;
        float c3 = (1 - x) * z * neighbor3Height;
        float c4 = x * z * neighbor4Height;

        return c1 + c2 + c3 + c4;
    }

    // MARK: - Private

    float Heightmap::getTileHeight_(int tileX, int tileZ)
    {
        if (tileX >= size_) return 0;
        if (tileZ >= size_) return 0;

        if (tileX < 0) return 0;
        if (tileZ < 0) return 0;

        return heights_[tileX][tileZ];
    }
}
