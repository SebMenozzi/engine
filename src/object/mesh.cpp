#include "mesh.h"

#ifndef BUFFER_OFFSET
    #define BUFFER_OFFSET(offset) ((const GLvoid *) (std::uintptr_t) (offset))
#endif

namespace object
{
    Mesh::Mesh() {}

    Mesh::Mesh(
        Buffer vertices,
        Buffer normals,
        Buffer uvs,
        Buffer indices
    ):
        vertices_(vertices),
        normals_(normals),
        uvs_(uvs),
        indices_(indices)
    {}

    Mesh::~Mesh()
    {
        glDeleteVertexArrays(1, &vaoID_);
        glDeleteBuffers(1, &vboID_);
        glDeleteBuffers(1, &eboID_);
    }

    void Mesh::load()
    {
        if (glIsBuffer(vboID_) == GL_TRUE)
            glDeleteBuffers(1, &vboID_);

        glGenVertexArrays(1, &vaoID_);
        glBindVertexArray(vaoID_);
            glGenBuffers(1, &vboID_);
            glBindBuffer(GL_ARRAY_BUFFER, vboID_);
                int verticesSize = vertices_.nbElements_ * vertices_.dataStride_;
                int normalsSize = normals_.nbElements_ * normals_.dataStride_;
                int uvsSize = uvs_.nbElements_ * uvs_.dataStride_;
                int indicesSize = indices_.nbElements_ * indices_.dataStride_;

                // Allocate to the GPU
                int totalSize = verticesSize + normalsSize + uvsSize;

                glBufferData(GL_ARRAY_BUFFER, totalSize, 0, GL_STATIC_DRAW);

                glBufferSubData(GL_ARRAY_BUFFER, 0,                          verticesSize, vertices_.data_);
                glBufferSubData(GL_ARRAY_BUFFER, verticesSize,               normalsSize,  normals_.data_);
                glBufferSubData(GL_ARRAY_BUFFER, verticesSize + normalsSize, uvsSize,      uvs_.data_);

                // Indices
                if (indicesSize > 0)
                {
                    glGenBuffers(1, &eboID_);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID_);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices_.data_, GL_STATIC_DRAW);
                }

                // Vertices
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
                glEnableVertexAttribArray(0);

                // Normals
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(verticesSize));
                glEnableVertexAttribArray(1);

                // Texture coordinates
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(verticesSize + normalsSize));
                glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Mesh::render()
    {   
        if (diffuseTexture_ != nullptr)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseTexture_->getID());
        }

        if (specularTexture_ != nullptr)
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularTexture_->getID());
        }

        if (normalTexture_ != nullptr)
        {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, normalTexture_->getID());
        }
    
        glBindVertexArray(vaoID_);
            if (indices_.data_ == nullptr)
                glDrawArrays(GL_TRIANGLES, 0, nbVertices());
            else
            {
                if (indices_.dataStride_ == sizeof(unsigned int))
                    glDrawElements(GL_TRIANGLES, nbIndices(), GL_UNSIGNED_INT, (void*) 0);
                else if (indices_.dataStride_ == sizeof(unsigned short))
                    glDrawElements(GL_TRIANGLES, nbIndices(), GL_UNSIGNED_SHORT, (void*) 0);
                else if (indices_.dataStride_ == sizeof(unsigned char))
                    glDrawElements(GL_TRIANGLES, nbIndices(), GL_UNSIGNED_BYTE, (void*) 0);
            }
        glBindVertexArray(0);
    }

    size_t Mesh::nbVertices()
    {
        return vertices_.nbElements_;
    }

    size_t Mesh::nbIndices()
    {
        return indices_.nbElements_;
    }

    void Mesh::setDiffuseTexture(texture::Texture* texture)
    {
        diffuseTexture_ = texture;
    }

    void Mesh::setSpecularTexture(texture::Texture* texture)
    {
        specularTexture_ = texture;
    }

    void Mesh::setNormalTexture(texture::Texture* texture)
    {
        normalTexture_ = texture;
    }

    void Mesh::setMaterial(material::Material* material)
    {
        material_ = material;
    }

    void Mesh::useShader(shader::Shader* shader)
    {
        if (material_ != nullptr)
        {
            shader->setVec3("ambientColor", material_->ka());
            shader->setVec3("diffuseColor", material_->kd());
            shader->setVec3("specularColor", material_->ks());
        }
        else
        {
            shader->setVec3("ambientColor", glm::vec3(0));
            shader->setVec3("diffuseColor", glm::vec3(0));
            shader->setVec3("specularColor", glm::vec3(0));
        }
    }
}