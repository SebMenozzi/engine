#pragma once

#include <vector>

#include "mesh.h"
#include "maths_utils.h"
#include "types.h"

namespace mesh
{
    class UVSphere: public Mesh
    {
        public:
            UVSphere(
                float radius, 
                int sectorCount, 
                int stackCount,
                const char* diffusePath, 
                const char* specularPath,
                const char* normalPath
            );

        protected:
            float radius_;
            int sectorCount_;
            int stackCount_;
    };
}