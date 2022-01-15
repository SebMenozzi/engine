#include "tiny_gltf_model.h"

namespace object
{
    // MARK: - Public 

    TinyGLTFModel::TinyGLTFModel(const std::string& filepath):
        filepath_(filepath),
        directory_(filepath.substr(0, filepath.find_last_of('/')) + '/')
    {
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;

        bool result = loader.LoadBinaryFromFile(&model, &err, &warn, filepath);
        
        if (!result || !err.empty())
        {
            std::cerr << err << std::endl;
            return;
        }

        loadMeshes_(&model);
    }

    void TinyGLTFModel::load()
    {
        for (size_t i = 0; i < meshes_.size(); ++i)
            meshes_[i]->load();
    }

    void TinyGLTFModel::render()
    {
        for (size_t i = 0; i < meshes_.size(); ++i)
            meshes_[i]->render();
    }

    void TinyGLTFModel::useShader(shader::Shader* shader)
    {
        for (size_t i = 0; i < meshes_.size(); ++i)
            meshes_[i]->useShader(shader);
    }

    // MARK: - Private

    uint32 TinyGLTFModel::calculateDataTypeSize_(const tinygltf::Accessor* accessor)
    {
        uint32 elementSize = 0;
        switch (accessor->componentType)
        {
        case TINYGLTF_PARAMETER_TYPE_BYTE:
        case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
            elementSize = 1;
            break;
        case TINYGLTF_PARAMETER_TYPE_SHORT:
        case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
            elementSize = 2;
            break;
        case TINYGLTF_PARAMETER_TYPE_FLOAT:
        case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
            elementSize = 4;
            break;
        }

        switch (accessor->type)
        {
        case TINYGLTF_PARAMETER_TYPE_FLOAT_MAT2:
            return 4 * TINYGLTF_PARAMETER_TYPE_FLOAT;
            break;
        case TINYGLTF_PARAMETER_TYPE_FLOAT_MAT3:
            return 9 * TINYGLTF_PARAMETER_TYPE_FLOAT;
            break;
        case TINYGLTF_PARAMETER_TYPE_FLOAT_MAT4:
            return 16 * TINYGLTF_PARAMETER_TYPE_FLOAT;
            break;
        case TINYGLTF_PARAMETER_TYPE_FLOAT_VEC2:
            return 2 * TINYGLTF_PARAMETER_TYPE_FLOAT;
            break;
        case TINYGLTF_PARAMETER_TYPE_FLOAT_VEC3:
            return 3 * TINYGLTF_PARAMETER_TYPE_FLOAT;
            break;
        case TINYGLTF_PARAMETER_TYPE_FLOAT_VEC4:
            return 4 * TINYGLTF_PARAMETER_TYPE_FLOAT;
            break;
        }

        return elementSize;
    }

    Buffer TinyGLTFModel::getData_(const tinygltf::Model* model, const tinygltf::Accessor* accessor)
    {
        const tinygltf::BufferView* bufferView = &model->bufferViews[accessor->bufferView];
        const tinygltf::Buffer* buffer = &model->buffers[bufferView->buffer];

        const uint32 dataTypeSize = calculateDataTypeSize_(accessor);

        // Allocate on the heap
        const uint8* data = &(buffer->data[bufferView->byteOffset + accessor->byteOffset]);
        void* dataCopy = malloc(accessor->count * dataTypeSize);
        memcpy(dataCopy, data, accessor->count * dataTypeSize);

        return Buffer { 
            reinterpret_cast<const uint8*>(dataCopy), 
            dataTypeSize, 
            accessor->count 
        };
    }

    void TinyGLTFModel::loadMeshes_(tinygltf::Model* model)
    {
        size_t meshCount = model->meshes.size();

        for (size_t i = 0; i < meshCount; ++i)
        {
            const tinygltf::Mesh mesh = model->meshes[i];

            for(size_t j = 0; j< mesh.primitives.size(); ++j)
		    {
                tinygltf::Primitive primitive = mesh.primitives[j];

                Buffer vertices;
                Buffer normals;
                Buffer tangents;
                Buffer uvs;
                Buffer indices;

                for (auto const& attrib : primitive.attributes)
                {
                    auto buffer = getData_(model, &model->accessors[attrib.second]);

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
                    indices = getData_(model, &model->accessors[primitive.indices]);
                    
                auto mesh = new Mesh(vertices, normals, uvs, indices);

                meshes_.push_back(mesh);
            }
        }
    }
}

/*
for (tinygltf::Node node : model.nodes)
{
    std::cout << node.name << std::endl;

    if (!node.translation.empty())
        std::cout << "translation: x " << node.translation[0] << " y " <<  node.translation[1] << " z " <<  node.translation[2] << std::endl;

    if (!node.rotation.empty())
        std::cout << "rotation: x " << node.rotation[0] << " y " <<  node.rotation[1] << " z " <<  node.rotation[2] << std::endl;

    if (!node.scale.empty())
        std::cout << "scale: x " << node.scale[0] << " y " <<  node.scale[1] << " z " <<  node.scale[2] << std::endl;


    if (node.mesh >= 0)
    {
        for (tinygltf::Primitive& primitive : model.meshes[node.mesh].primitives)
        {

            for (auto[attribName, accessorId] : primitive.attributes)
            {
                std::cout << attribName << std::endl;

                tinygltf::Accessor const& gltf_accessor = model.accessors[accessorId];
            }
        }
    }
}
*/