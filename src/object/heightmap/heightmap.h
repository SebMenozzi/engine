#pragma once

#include <vector>

#include "mesh.h"
#include "types.h"

namespace object
{
    class Heightmap : public Mesh
    {
        public:
            Heightmap(uint32 size);
        protected:
            uint32 size_;
    };
}