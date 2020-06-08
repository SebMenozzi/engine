#include "texturedMesh.h"

#ifndef BUFFER_OFFSET
    #define BUFFER_OFFSET(offset) ((char*) 0 + offset)
#endif

TexturedMesh::TexturedMesh(const char* diffusePath, const char* specularPath)
: Mesh(), diffuse(diffusePath), specular(specularPath)
{
    this->diffuse.load();
    this->specular.load();
}

void TexturedMesh::load()
{
    if (glIsBuffer(this->vboID) == GL_TRUE)
        glDeleteBuffers(1, &this->vboID);

    glGenVertexArrays(1, &this->vaoID);
    glBindVertexArray(this->vaoID);
        glGenBuffers(1, &this->vboID);
        glBindBuffer(GL_ARRAY_BUFFER, this->vboID);
            int verticesSize = this->vertices.size() * sizeof(float);
            int normalsSize = this->normals.size() * sizeof(float);
            int textureCoordSize = this->textureCoord.size() * sizeof(float);

            // Allocate to the GPU
            glBufferData(GL_ARRAY_BUFFER, verticesSize + normalsSize + textureCoordSize, 0, GL_STATIC_DRAW);

            glBufferSubData(GL_ARRAY_BUFFER, 0,                          verticesSize, &this->vertices[0]);
            glBufferSubData(GL_ARRAY_BUFFER, verticesSize,               normalsSize,   &this->normals[0]);
            glBufferSubData(GL_ARRAY_BUFFER, verticesSize + normalsSize, textureCoordSize,  &this->textureCoord[0]);

            // vertices
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);

            // normals
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(verticesSize));
            glEnableVertexAttribArray(1);

            // texture coords
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(verticesSize + normalsSize));
            glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TexturedMesh::render()
{
    // bind diffuse map
    //glEnable(GL_FRAMEBUFFER_SRGB);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->diffuse.getID());
    //glDisable(GL_FRAMEBUFFER_SRGB);

    /*
    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->specular.getID());
    */

    glBindVertexArray(this->vaoID);
    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() / 3);
    glBindVertexArray(0);
}
