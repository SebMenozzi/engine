#include "model.h"

namespace mesh
{
    // MARK: - Public 

    Model::Model(const std::string& filepath)
    {
        // http://assimp.sourceforge.net/lib_html/usage.html
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(
            filepath, 
            aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs
        );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cerr << importer.GetErrorString() << std::endl;
            return;
        }

        directory_ = filepath.substr(0, filepath.find_last_of('/')) + '/';

        loadMeshes_(scene->mRootNode, scene);
    }

    void Model::load()
    {
        for (auto& mesh: meshes_)
            mesh.load();
    }

    void Model::render()
    {
        for (auto& mesh: meshes_)
            mesh.render();
    }

    // MARK: - Private 

    void Model::loadMeshes_(aiNode* root, const aiScene* scene)
    {
        for (unsigned int i = 0; i < root->mNumMeshes; ++i)
        {
            aiMesh* aiMesh = scene->mMeshes[root->mMeshes[i]];
            aiMaterial* aiMaterial = scene->mMaterials[aiMesh->mMaterialIndex];

            auto mesh = convertMesh_(aiMesh, aiMaterial);
            meshes_.push_back(mesh);

            //materials_.push_back(load_material(material));
        }

        for (unsigned int i = 0; i < root->mNumChildren; ++i)
            loadMeshes_(root->mChildren[i], scene);
    }

    std::string Model::getTextureName_(
        const aiMaterial* aiMaterial, 
        aiTextureType type
    ) 
    {
        aiString textureName;
        aiMaterial->GetTexture(type, 0, &textureName);
        
        std::string str = textureName.C_Str();
        if (str.empty())
            return "";
        
        return directory_ + str;
    }

    Mesh Model::convertMesh_(const aiMesh* aiMesh, const aiMaterial* aiMaterial)
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;

        for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i)
        {
            // Vertices
            const aiVector3t<float>& v = aiMesh->mVertices[i];
            vertices.push_back(glm::vec3(v.x, v.y, v.z));
            
            // Normals
            if (aiMesh->HasNormals())
            {
                const aiVector3t<float>& n = aiMesh->mNormals[i];
                normals.push_back(glm::vec3(n.x, n.y, n.z));
            }

            // Uvs
            if (aiMesh->HasTextureCoords(0))
            {
                const aiVector3t<float>& uv = aiMesh->mTextureCoords[0][i];
                uvs.push_back(glm::vec2(uv.x, uv.y));
            }
        }

        // Indices
        std::vector<unsigned int> indices;
        for (unsigned int i = 0; i < aiMesh->mNumFaces; ++i)
            for (unsigned int j = 0; j < aiMesh->mFaces[i].mNumIndices; ++j)
                indices.push_back(aiMesh->mFaces[i].mIndices[j]);

        auto mesh = Mesh(vertices, normals, uvs, indices);

        /*
        // Diffuse texture
        std::string diffusePath = getTextureName_(aiMaterial, aiTextureType_DIFFUSE);
        if (!diffusePath.empty())
        {
            if (loadedTextures_.find(diffusePath) == loadedTextures_.end())
                loadedTextures_[diffusePath] = texture::Texture(diffusePath.c_str());

            mesh.setDiffuseTexture(loadedTextures_[diffusePath]);
        }

        // Specular texture
        std::string specularPath = getTextureName_(aiMaterial, aiTextureType_SPECULAR);
        if (!specularPath.empty())
        {
            if (loadedTextures_.find(specularPath) == loadedTextures_.end())
                loadedTextures_[specularPath] = texture::Texture(specularPath.c_str());

            mesh.setSpecularTexture(loadedTextures_[specularPath]);
        }

        // Normal texture
        std::string normalPath = getTextureName_(aiMaterial, aiTextureType_HEIGHT);
        if (!normalPath.empty())
        {
            if (loadedTextures_.find(normalPath) == loadedTextures_.end())
                loadedTextures_[normalPath] = texture::Texture(normalPath.c_str());

            mesh.setNormalTexture(loadedTextures_[normalPath]);
        }
        */

        return mesh;
    }
}