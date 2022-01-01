#pragma once

#include <vector>

#include "mesh.h"
#include "maths_utils.h"
#include "types.h"

namespace object
{
    class UVSphere: public Mesh
    {
        public:
            UVSphere(
                float radius, 
                int sectorCount, 
                int stackCount
            );

        protected:
            float radius_;
            int sectorCount_;
            int stackCount_;
    };
}