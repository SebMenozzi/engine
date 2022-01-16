#pragma once

#include <iostream>
#include <assimp/Importer.hpp>   // C++ importer interface
#include <assimp/scene.h>        // Output data structure
#include <assimp/postprocess.h>  // Post processing flags
#include <string>
#include <unordered_map>

#include "texture.h"
#include "material.h"
#include "mesh.h"
#include "object.h"

namespace object
{
    class AssimpModel: public Object
    {
        public:
            AssimpModel(const std::string& filepath);

            void load() override;
            void render() override;
            void useShader(shader::Shader* shader) override;
        private:
            void loadTextures_(const aiScene* scene);
            void loadMeshes_(aiNode* root, const aiScene* scene);
            Mesh* convertMesh_(const aiMesh* aiMesh, const aiMaterial* aiMaterial);
            texture::Texture* getTexture_(const std::string& filepath, const std::string& name);
            std::string getTexturePath_(const aiMaterial* aiMaterial,  aiTextureType type);
            void addMaterialToMesh_(Mesh* mesh, const aiMaterial* aiMaterial);
            void addTextureToMesh_(Mesh* mesh, const std::string& filepath);

            std::string filepath_;
            std::string directory_;
            std::vector<Mesh*> meshes_;
            std::unordered_map<std::string, texture::Texture*> textures_;
    };
}