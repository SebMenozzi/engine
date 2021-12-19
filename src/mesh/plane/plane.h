#pragma once

#include "mesh.h"

namespace mesh
{
    class Plane : public Mesh
    {
        public:
            Plane(
                float size,
                const char* diffusePath, 
                const char* specularPath,
                const char* normalPath
            );
    };
}