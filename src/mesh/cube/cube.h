#pragma once

#include "mesh.h"

namespace mesh
{
    class Cube: public Mesh
    {
        public:
            Cube(
                float size, 
                const char* diffusePath, 
                const char* specularPath,
                const char* normalPath
            );
    };
}