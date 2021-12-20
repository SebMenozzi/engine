#pragma once

#include <assimp/Importer.hpp>   // C++ importer interface
#include <assimp/scene.h>        // Output data structure
#include <assimp/postprocess.h>  // Post processing flags

#include <string>
#include <unordered_map>

#include "texture.h"
#include "material.h"
#include "mesh.h"

namespace mesh
{
    class Model
    {
        public:
            Model(const std::string& filepath);

            void load();
            void render();
        private:
            void loadMeshes_(aiNode* root, const aiScene* scene);
            Mesh convertMesh_(const aiMesh* aiMesh, const aiMaterial* aiMaterial);
            std::string getTextureName_(const aiMaterial* aiMaterial,  aiTextureType type);

            std::vector<Mesh> meshes_;
            std::vector<material::Material> materials_;
            std::unordered_map<std::string, texture::Texture> loadedTextures_;

            std::string directory_;
    };
}