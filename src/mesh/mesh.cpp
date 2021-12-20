#include "mesh.h"

#ifndef BUFFER_OFFSET
    #define BUFFER_OFFSET(offset) ((const GLvoid *) (std::uintptr_t) (offset))
#endif

namespace mesh
{
    Mesh::Mesh() {}

    Mesh::Mesh(
        std::vector<glm::vec3> vertices,
        std::vector<glm::vec3> normals,
        std::vector<glm::vec2> uvs,
        std::vector<unsigned int> indices
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

        // Clear memory
        vertices_.clear();
        normals_.clear();
        uvs_.clear();
        indices_.clear();
    }

    void Mesh::load()
    {
        if (glIsBuffer(vboID_) == GL_TRUE)
            glDeleteBuffers(1, &vboID_);

        glGenVertexArrays(1, &vaoID_);
        glBindVertexArray(vaoID_);
            glGenBuffers(1, &vboID_);
            glBindBuffer(GL_ARRAY_BUFFER, vboID_);
                int verticesSize = vertices_.size() * sizeof(glm::vec3);
                int normalsSize = normals_.size() * sizeof(glm::vec3);
                int uvsSize = uvs_.size() * sizeof(glm::vec2);
                int indicesSize = indices_.size() * sizeof(unsigned int);

                // Allocate to the GPU
                int totalSize = verticesSize + normalsSize + uvsSize;

                glBufferData(GL_ARRAY_BUFFER, totalSize, 0, GL_STATIC_DRAW);

                glBufferSubData(GL_ARRAY_BUFFER, 0,                          verticesSize, &vertices_[0]);
                glBufferSubData(GL_ARRAY_BUFFER, verticesSize,               normalsSize,  &normals_[0]);
                glBufferSubData(GL_ARRAY_BUFFER, verticesSize + normalsSize, uvsSize,      &uvs_[0]);

                // Indices
                if (indicesSize > 0)
                {
                    glGenBuffers(1, &eboID_);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID_);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, &indices_[0], GL_STATIC_DRAW);
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
        if (diffuseTexture_.has_value())
        {
            // Bind diffuse map
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseTexture_.value().getID());
        }

        if (specularTexture_.has_value())
        {
            // Bind specular map
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularTexture_.value().getID());
        }

        if (normalTexture_.has_value())
        {
            // Bind normal map
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, normalTexture_.value().getID());
        }

        glBindVertexArray(vaoID_);
        if (indices_.size() == 0)
            glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
        else
            glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, (void*) 0);
        glBindVertexArray(0);
    }

    size_t Mesh::nbVertices()
    {
        return vertices_.size();
    }

    void Mesh::setDiffuseTexture(texture::Texture diffuse)
    {
        diffuseTexture_ = std::make_optional(diffuse);
    }
    
    void Mesh::setSpecularTexture(texture::Texture specular)
    {
        specularTexture_ = std::make_optional(specular);
    }

    void Mesh::setNormalTexture(texture::Texture normal)
    {
        normalTexture_ = std::make_optional(normal);
    }
}