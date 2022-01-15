#include "uv_sphere.h"

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;

// From http://www.songho.ca/opengl/gl_sphere.html

namespace object
{
    UVSphere::UVSphere(
        float radius,
        int sectorCount,
        int stackCount
    ):
        Mesh(),
        radius_(radius),
        sectorCount_((sectorCount < MIN_SECTOR_COUNT) ? MIN_SECTOR_COUNT : sectorCount),
        stackCount_((stackCount < MIN_STACK_COUNT) ? MIN_STACK_COUNT : stackCount) 
    {
        auto vertices = new std::vector<glm::vec3>();
        auto normals = new std::vector<glm::vec3>();
        auto uvs = new std::vector<glm::vec2>();
        auto indices = new std::vector<unsigned int>();

        // Generate vertices, normals and texture coords
        float lengthInv = 1.0f / radius_;
        float sectorStep = 2 * M_PI / sectorCount_;
        float stackStep = M_PI / stackCount_;

        for (int i = 0; i <= stackCount_; ++i)
        {
            float stackAngle = M_PI / 2 - i * stackStep; // Starting from pi/2 to -pi/2

            float xy = radius_ * cosf(stackAngle); // r * cos(u)
            float z = radius_ * sinf(stackAngle);  // r * sin(u)

            for (int j = 0; j <= sectorCount_; ++j)
            {
                float sectorAngle = j * sectorStep; // starting from 0 to 2pi

                // Vertices
                float x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
                float y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)

                vertices->push_back(glm::vec3(x, y, z));

                // Normals
                float nx = x * lengthInv;
                float ny = y * lengthInv;
                float nz = z * lengthInv;

                normals->push_back(glm::vec3(nx, ny, nz));

                // Texture coords
                float s = (float) j / sectorCount_;
                float t = (float) i / stackCount_;
                
                uvs->push_back(glm::vec2(s, t));
            }
        }

        // Generate indices

        //  k1--k1+1
        //  |  / |
        //  | /  |
        //  k2--k2+1

        for (int i = 0; i < stackCount_; ++i)
        {
            int k1 = i * (sectorCount + 1); // beginning of current stack
            int k2 = k1 + sectorCount + 1;  // beginning of next stack

            for (int j = 0; j < sectorCount_; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1 + 1
                if (i != 0)
                {
                    indices->push_back(k1);
                    indices->push_back(k2);
                    indices->push_back(k1 + 1);
                }

                // k1 + 1 => k2 => k2 + 1
                if (i != (stackCount_ - 1))
                {
                    indices->push_back(k1 + 1);
                    indices->push_back(k2);
                    indices->push_back(k2 + 1);
                }
            }
        }

        vertices_ = Buffer { reinterpret_cast<const uint8*>(vertices->data()), sizeof(glm::vec3), vertices->size() };
        normals_ = Buffer { reinterpret_cast<const uint8*>(normals->data()), sizeof(glm::vec3), normals->size() };
        uvs_ = Buffer { reinterpret_cast<const uint8*>(uvs->data()), sizeof(glm::vec2), uvs->size() };
        indices_ = Buffer { reinterpret_cast<const uint8*>(indices->data()), sizeof(unsigned int), indices->size() };
    }
}