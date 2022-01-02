#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <tiny_gltf.h>

#include "texture.h"
#include "material.h"
#include "mesh.h"
#include "object.h"
#include "types.h"

namespace object
{
    class TinyGLTFModel: public Object
    {
        public:
            TinyGLTFModel(const std::string& filepath);

            void load() override;
            void render() override;
            void useShader(shader::Shader* shader) override;
        private:
            uint32 calculateDataTypeSize_(const tinygltf::Accessor* accessor);
            Buffer getData_(const tinygltf::Model* model, const tinygltf::Accessor* accessor);
            void loadMeshes_(tinygltf::Model* model);

            std::string filepath_;
            std::string directory_;
            std::vector<Mesh*> meshes_;
            std::unordered_map<std::string, texture::Texture*> textures_;
    };
}