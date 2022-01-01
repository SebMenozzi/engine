#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include "gltf.h"

#include "texture.h"
#include "material.h"
#include "mesh.h"
#include "object.h"
#include "types.h"

namespace object
{
    class FxGLTFModel: public Object
    {
        public:
            FxGLTFModel(const std::string& filepath);

            void load() override;
            void render() override;
            void useShader(shader::Shader* shader) override;
        private:
            void loadTextures_(const fx::gltf::Document* scene);
            uint32 calculateDataTypeSize_(const fx::gltf::Accessor* accessor) noexcept;
            Buffer getData_(const fx::gltf::Document* scene, const fx::gltf::Accessor* accessor);
            void loadMeshes_(const fx::gltf::Document* scene);

            std::string filepath_;
            std::string directory_;
            std::vector<Mesh*> meshes_;
            std::unordered_map<size_t, texture::Texture*> textures_;
    };
}