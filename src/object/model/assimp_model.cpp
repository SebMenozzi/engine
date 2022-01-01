#include "assimp_model.h"

namespace object
{
    // MARK: - Public 

    AssimpModel::AssimpModel(const std::string& filepath):
        filepath_(filepath),
        directory_(filepath.substr(0, filepath.find_last_of('/')) + '/')
    {
        // http://assimp.sourceforge.net/lib_html/usage.html
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(
            filepath, 
            aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_GenSmoothNormals
        );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cerr << importer.GetErrorString() << std::endl;
            return;
        }

        loadTextures_(scene);
        loadMeshes_(scene->mRootNode, scene);

        if (scene->mAnimations != nullptr)
        {
            float ticksPerSecond = scene->mAnimations[0]->mTicksPerSecond;
            float animationTime = scene->mAnimations[0]->mDuration;

            std::cout << "Ticks per second: " << ticksPerSecond << std::endl;
            std::cout << "Animation time: " << animationTime << std::endl;
        }
    }

    void AssimpModel::load()
    {
        for (size_t i = 0; i < meshes_.size(); ++i)
            meshes_[i]->load();
    }

    void AssimpModel::render()
    {
        for (size_t i = 0; i < meshes_.size(); ++i)
            meshes_[i]->render();
    }

    void AssimpModel::useShader(shader::Shader* shader)
    {
        for (size_t i = 0; i < meshes_.size(); ++i)
            meshes_[i]->useShader(shader);
    }

    // MARK: - Private 

    void AssimpModel::loadTextures_(const aiScene* scene)
    {
        if (scene->HasTextures())
        {
            for(size_t i = 0; i < scene->mNumTextures; ++i)
		    {
                size_t width = scene->mTextures[i]->mWidth;
                size_t height = scene->mTextures[i]->mHeight;

                SDL_Surface* image = nullptr;

                // If this value is zero, pcData points to an compressed texture
                if (height == 0)
                {
                    void* ptr = scene->mTextures[i]->pcData;

                    if (ptr == nullptr)
                        continue;

                    SDL_RWops* rw = SDL_RWFromConstMem(ptr, width);
                    image = IMG_Load_RW(rw, 1);
                }
                else
                {
                    image = utils::createImage(width, height);

                    for (size_t y = 0; y < height; ++y)
                    {
                        for (size_t x = 0; x < width; ++x)
                        {
                            aiTexel texel = scene->mTextures[i]->pcData[x * width + y];
                            uint8 r = texel.r;
                            uint8 g = texel.g;
                            uint8 b = texel.b;

                            Uint32 pixel = SDL_MapRGB(image->format, r, g, b);
                            utils::putPixel(image, x, y, pixel);
                        }
                    }
                }

                std::string iStr = std::to_string(i);

                auto texture = new texture::Texture(image);
                textures_[directory_ + "*" + iStr] = texture;
                texture->load();

                std::string filename = filepath_.substr(filepath_.find_last_of("/\\") + 1);
                std::string outputPath = "build/" + filename + "." + iStr + ".png";
                IMG_SavePNG(image, outputPath.c_str());
            }
        }
    }

    void AssimpModel::loadMeshes_(aiNode* root, const aiScene* scene)
    {
        for (unsigned int i = 0; i < root->mNumMeshes; ++i)
        {
            aiMesh* aiMesh = scene->mMeshes[root->mMeshes[i]];
            aiMaterial* aiMaterial = scene->mMaterials[aiMesh->mMaterialIndex];

            Mesh* mesh = convertMesh_(aiMesh, aiMaterial);

            meshes_.push_back(mesh);
        }

        for (unsigned int i = 0; i < root->mNumChildren; ++i)
            loadMeshes_(root->mChildren[i], scene);
    }

    std::string AssimpModel::getTextureName_(
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

    texture::Texture* AssimpModel::getTexture_(const std::string& filepath)
    {
        texture::Texture* texture;

        if (textures_[filepath])
            texture = textures_[filepath.c_str()];
        else
        {
            texture = new texture::Texture(filepath.c_str());
            texture->load();
        }

        return texture;
    }

    void AssimpModel::addMaterialToMesh_(Mesh* mesh, const aiMaterial* aiMaterial)
    {
        aiColor3D color;
        aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
        auto ka = glm::vec3(color.r, color.g, color.b);

        aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        auto kd = glm::vec3(color.r, color.g, color.b);

        aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
        auto ks = glm::vec3(color.r, color.g, color.b);

        auto material = new material::Material(ka, kd, ks);

        mesh->setMaterial(material);
    }

    Mesh* AssimpModel::convertMesh_(const aiMesh* aiMesh, const aiMaterial* aiMaterial)
    {
        auto vertices = new std::vector<glm::vec3>();
        auto normals = new std::vector<glm::vec3>();
        auto uvs = new std::vector<glm::vec2>();
        auto indices = new std::vector<unsigned int>();

        for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i)
        {
            // Vertices
            const aiVector3t<float>& v = aiMesh->mVertices[i];
            vertices->push_back(glm::vec3(v.x, v.y, v.z));
            
            // Normals
            const aiVector3t<float>& n = aiMesh->mNormals[i];
            normals->push_back(glm::vec3(n.x, n.y, n.z));

            // Uvs
            const aiVector3t<float>& uv = aiMesh->mTextureCoords[0][i];
            uvs->push_back(glm::vec2(uv.x, uv.y));
        }

        // Indices
        for (unsigned int i = 0; i < aiMesh->mNumFaces; ++i)
            for (unsigned int j = 0; j < aiMesh->mFaces[i].mNumIndices; ++j)
                indices->push_back(aiMesh->mFaces[i].mIndices[j]);

        auto verticesBuffer = Buffer { reinterpret_cast<const uint8*>(vertices->data()), sizeof(glm::vec3), vertices->size() };
        auto normalsBuffer = Buffer { reinterpret_cast<const uint8*>(normals->data()), sizeof(glm::vec3), normals->size() };
        auto uvsBuffer = Buffer { reinterpret_cast<const uint8*>(uvs->data()), sizeof(glm::vec2), uvs->size() };
        auto indicesBuffer = Buffer { reinterpret_cast<const uint8*>(indices->data()), sizeof(unsigned int), indices->size() };

        auto mesh = new Mesh(verticesBuffer, normalsBuffer, uvsBuffer, indicesBuffer);

        // Textures

        std::string diffusePath = getTextureName_(aiMaterial, aiTextureType_DIFFUSE);
        if (!diffusePath.empty())
        {
            auto diffuseTexture = getTexture_(diffusePath);
            mesh->setDiffuseTexture(diffuseTexture);
        }

        std::string specularPath = getTextureName_(aiMaterial, aiTextureType_SPECULAR);
        if (!specularPath.empty())
        {
            auto specularTexture = getTexture_(specularPath);
            mesh->setSpecularTexture(specularTexture);
        }

        std::string normalPath = getTextureName_(aiMaterial, aiTextureType_NORMALS);
        if (!normalPath.empty())
        {
            auto normalTexture = getTexture_(normalPath);
            mesh->setNormalTexture(normalTexture);
        }

        // Material
        addMaterialToMesh_(mesh, aiMaterial);

        return mesh;
    }
}