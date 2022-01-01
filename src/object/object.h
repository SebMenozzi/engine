#pragma once

#include "shader.h"

namespace object
{
    class Object
    {
        public:
            virtual ~Object() {}

            virtual void load() = 0;
            virtual void render() = 0;
            virtual void useShader(shader::Shader* shader) = 0;
    };
}