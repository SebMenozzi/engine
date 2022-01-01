#include "fx_gltf_model.h"

namespace object
{
    // MARK: - Public 

    FxGLTFModel::FxGLTFModel(const std::string& filepath):
        filepath_(filepath),
        directory_(filepath.substr(0, filepath.find_last_of('/')) + '/')
    {
        const fx::gltf::Document scene = fx::gltf::LoadFromBinary(filepath);

        loadTextures_(&scene);
        loadMeshes_(&scene);
    }

     void FxGLTFModel::load()
    {
        for (size_t i = 0; i < meshes_.size(); ++i)
            meshes_[i]->load();
    }

    void FxGLTFModel::render()
    {
        for (size_t i = 0; i < meshes_.size(); ++i)
            meshes_[i]->render();
    }

    void FxGLTFModel::useShader(shader::Shader* shader)
    {
        for (size_t i = 0; i < meshes_.size(); ++i)
            meshes_[i]->useShader(shader);
    }

    // MARK: - Private

    void FxGLTFModel::loadTextures_(const fx::gltf::Document* scene)
    {
        for (size_t i = 0; i < scene->textures.size(); ++i)
        {
            const fx::gltf::Image& image = scene->images[scene->textures[i].source];

            const bool isEmbedded = image.IsEmbeddedResource();

            texture::Texture* texture;

            if (!image.uri.empty() && !isEmbedded)
            {
                std::string texturePath = fx::gltf::detail::GetDocumentRootPath(directory_) / image.uri;

                texture = new texture::Texture(texturePath.c_str());
                textures_[i] = texture;
                texture->load();
            }
            else
            {
                std::vector<uint8_t> imageData;
                SDL_RWops* rw = nullptr;

                if (isEmbedded)
                {
                    image.MaterializeData(imageData);
                    rw = SDL_RWFromConstMem(&imageData[0], imageData.size());
                }
                else
                {
                    const fx::gltf::BufferView& bufferView = scene->bufferViews[image.bufferView];
                    const fx::gltf::Buffer& buffer = scene->buffers[bufferView.buffer];

                    rw = SDL_RWFromConstMem(&buffer.data[bufferView.byteOffset], bufferView.byteLength);
                }

                SDL_Surface* surface = IMG_Load_RW(rw, 1);

                auto texture = new texture::Texture(surface);
                textures_[i] = texture;
                texture->load();

                std::string filename = filepath_.substr(filepath_.find_last_of("/\\") + 1);
                std::string outputPath = "build/" + filename + "." + std::to_string(i) + ".png";
                IMG_SavePNG(surface, outputPath.c_str());
            }
        }
    }

    uint32 FxGLTFModel::calculateDataTypeSize_(const fx::gltf::Accessor* accessor) noexcept
    {
        uint32 elementSize = 0;
        switch (accessor->componentType)
        {
        case fx::gltf::Accessor::ComponentType::Byte:
        case fx::gltf::Accessor::ComponentType::UnsignedByte:
            elementSize = 1;
            break;
        case fx::gltf::Accessor::ComponentType::Short:
        case fx::gltf::Accessor::ComponentType::UnsignedShort:
            elementSize = 2;
            break;
        case fx::gltf::Accessor::ComponentType::Float:
        case fx::gltf::Accessor::ComponentType::UnsignedInt:
            elementSize = 4;
            break;
        }

        switch (accessor->type)
        {
        case fx::gltf::Accessor::Type::Mat2:
            return 4 * elementSize;
            break;
        case fx::gltf::Accessor::Type::Mat3:
            return 9 * elementSize;
            break;
        case fx::gltf::Accessor::Type::Mat4:
            return 16 * elementSize;
            break;
        case fx::gltf::Accessor::Type::Scalar:
            return elementSize;
            break;
        case fx::gltf::Accessor::Type::Vec2:
            return 2 * elementSize;
            break;
        case fx::gltf::Accessor::Type::Vec3:
            return 3 * elementSize;
            break;
        case fx::gltf::Accessor::Type::Vec4:
            return 4 * elementSize;
            break;
        }

        return 0;
    }

    Buffer FxGLTFModel::getData_(const fx::gltf::Document* scene, const fx::gltf::Accessor* accessor)
    {
        const fx::gltf::BufferView* bufferView = &scene->bufferViews[accessor->bufferView];
        const fx::gltf::Buffer* buffer = &scene->buffers[bufferView->buffer];

        const uint32 dataTypeSize = calculateDataTypeSize_(accessor);

        // Allocate on the heap
        const uint8* data = &(buffer->data[bufferView->byteOffset + accessor->byteOffset]);
        uint8* dataCopy = (uint8*) malloc(accessor->count * dataTypeSize);
        memcpy(dataCopy, data, accessor->count * dataTypeSize);

        return Buffer { 
            reinterpret_cast<const uint8*>(dataCopy), 
            dataTypeSize, 
            accessor->count 
        };
    }

    void FxGLTFModel::loadMeshes_(const fx::gltf::Document* scene)
    {
        for (size_t i = 0; i < scene->meshes.size(); i++)
        {
            const fx::gltf::Mesh& mesh = scene->meshes[i];

            for (size_t j = 0; j < mesh.primitives.size(); j++)
            {
                const fx::gltf::Primitive& primitive = mesh.primitives[j];

                Buffer vertices;
                Buffer normals;
                Buffer tangents;
                Buffer uvs;
                Buffer indices;

                for (auto const& attrib : primitive.attributes)
                {
                    auto buffer = getData_(scene, &scene->accessors[attrib.second]);

                    if (attrib.first == "POSITION")
                        vertices = buffer;
                    else if (attrib.first == "NORMAL")
                        normals = buffer;
                    else if (attrib.first == "TANGENT")
                        tangents = buffer;
                    else if (attrib.first == "TEXCOORD_0")
                        uvs = buffer;
                }

                if (primitive.indices >= 0)
                    indices = getData_(scene, &scene->accessors[primitive.indices]);
                
                auto mesh = new Mesh(vertices, normals, uvs, indices);

                meshes_.push_back(mesh);

                // Textures
                if (primitive.material >= 0)
                {
                    auto material = scene->materials[primitive.material];

                    auto diffuseTexture = textures_[material.pbrMetallicRoughness.baseColorTexture.index];
                    mesh->setDiffuseTexture(diffuseTexture);
                }
            }
        }
    }
}