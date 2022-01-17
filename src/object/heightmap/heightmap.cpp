#include "heightmap.h"

namespace object
{
    Heightmap::Heightmap(
        uint32 size,
        float scale
    ): 
        Mesh(),
        size_(size),
        scale_(scale)
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
        if (tileX >= size_) return heights_[size_ - 1][size_ - 1];
        if (tileZ >= size_) return heights_[size_ - 1][size_ - 1];

        if (tileX < 0) return heights_[0][0];
        if (tileZ < 0) return heights_[0][0];

        return heights_[tileX][tileZ];
    }

    void Heightmap::computeNormals_(float strength)
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

            nVec3[0] = computeNormal_(x, z, strength);
            nVec3[1] = computeNormal_(x + 1, z, strength);
            nVec3[2] = computeNormal_(x, z + 1, strength);
            nVec3[3] = computeNormal_(x, z + 1, strength);
            nVec3[4] = computeNormal_(x + 1, z + 1, strength);
            nVec3[5] = computeNormal_(x + 1, z, strength);

            n += normals_.dataStride_ * 6;
            p += vertices_.dataStride_ * 6;
        }
    }

    glm::vec3 Heightmap::computeNormal_(int x, int z, float strength)
    {
        if (x >= size_)
            return computeNormal_(x - 1, z, strength);
        if (z >= size_)
            return computeNormal_(x, z - 1, strength);

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
}
