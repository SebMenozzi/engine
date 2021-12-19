#include "uv_sphere.h"

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;

// From http://www.songho.ca/opengl/gl_sphere.html

namespace mesh
{
    UVSphere::UVSphere(
        float radius,
        int sectorCount, 
        int stackCount,
        const char* diffusePath, 
        const char* specularPath,
        const char* normalPath
    ):
        Mesh(diffusePath, specularPath, normalPath),
        radius_(radius),
        sectorCount_((sectorCount < MIN_SECTOR_COUNT) ? MIN_SECTOR_COUNT : sectorCount),
        stackCount_((stackCount < MIN_STACK_COUNT) ? MIN_STACK_COUNT : stackCount) 
    {
        // Generate vertices, normals and texture coords
        float lengthInv = 1.0f / radius_;
        float sectorStep = 2 * utils::PI / sectorCount_;
        float stackStep = utils::PI / stackCount_;

        for (int i = 0; i <= stackCount_; ++i) {
            float stackAngle = utils::PI / 2 - i * stackStep; // Starting from pi/2 to -pi/2

            float xy = radius_ * cosf(stackAngle); // r * cos(u)
            float z = radius_ * sinf(stackAngle);  // r * sin(u)

            for (int j = 0; j <= sectorCount_; ++j) {
                float sectorAngle = j * sectorStep; // starting from 0 to 2pi

                // Vertices
                float x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
                float y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)

                vertices_.push_back(x);
                vertices_.push_back(y);
                vertices_.push_back(z);

                // Normals
                float nx = x * lengthInv;
                float ny = y * lengthInv;
                float nz = z * lengthInv;

                normals_.push_back(nx);
                normals_.push_back(ny);
                normals_.push_back(nz);

                // Texture coords
                float s = (float) j / sectorCount_;
                float t = (float) i / stackCount_;
                uvs_.push_back(s);
                uvs_.push_back(t);
            }
        }

        // Generate indices

        //  k1--k1+1
        //  |  / |
        //  | /  |
        //  k2--k2+1

        for (int i = 0; i < stackCount_; ++i) {
            int k1 = i * (sectorCount + 1); // beginning of current stack
            int k2 = k1 + sectorCount + 1;  // beginning of next stack

            for (int j = 0; j < sectorCount_; ++j, ++k1, ++k2) {
                // 2 triangles per sector excluding first and last stacks

                // k1 => k2 => k1 + 1
                if (i != 0) {
                    indices_.push_back(k1);
                    indices_.push_back(k2);
                    indices_.push_back(k1 + 1);
                }

                // k1 + 1 => k2 => k2 + 1
                if (i != (stackCount_ - 1)) {
                    indices_.push_back(k1 + 1);
                    indices_.push_back(k2);
                    indices_.push_back(k2 + 1);
                }
            }
        }
    }
}