#include "mesh.h"

#ifndef BUFFER_OFFSET
    #define BUFFER_OFFSET(offset) ((char*) 0 + offset)
#endif

namespace mesh
{
    Mesh::Mesh(
        const char* diffusePath, 
        const char* specularPath,
        const char* normalPath
    ):
        vaoID_(0),
        vboID_(0),
        eboID_(0),
        diffuse_(diffusePath), 
        specular_(specularPath),
        normal_(normalPath)
    {
        diffuse_.load();
        specular_.load();
        normal_.load();
    }

    Mesh::~Mesh()
    {
        glDeleteVertexArrays(1, &vaoID_);
        glDeleteBuffers(1, &vboID_);
        glDeleteBuffers(1, &eboID_);

        // Clear memory
        vertices_.clear();
        indices_.clear();
        normals_.clear();
        uvs_.clear();
    }

    void Mesh::load()
    {
        if (glIsBuffer(vboID_) == GL_TRUE)
            glDeleteBuffers(1, &vboID_);

        glGenVertexArrays(1, &vaoID_);
        glBindVertexArray(vaoID_);
            glGenBuffers(1, &vboID_);
            glBindBuffer(GL_ARRAY_BUFFER, vboID_);
                int verticesSize = vertices_.size() * sizeof(float);
                int normalsSize = normals_.size() * sizeof(float);
                int uvsSize = uvs_.size() * sizeof(float);
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
        // Bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuse_.getID());

        // Bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specular_.getID());

        // Bind normal map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specular_.getID());

        glBindVertexArray(vaoID_);
        if (indices_.size() == 0)
            glDrawArrays(GL_TRIANGLES, 0, vertices_.size() / 3);
        else
            glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, (void*) 0);
        glBindVertexArray(0);
    }
}