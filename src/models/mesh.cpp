#include "mesh.h"

#ifndef BUFFER_OFFSET
    #define BUFFER_OFFSET(offset) ((char*) 0 + offset)
#endif

Mesh::Mesh()
{
    this->vaoID = 0;
    this->vboID = 0;
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &this->vaoID);
    glDeleteBuffers(1, &this->vboID);
}

void Mesh::load()
{
    if(glIsBuffer(this->vboID) == GL_TRUE)
        glDeleteBuffers(1, &this->vboID);

    glGenVertexArrays(1, &this->vaoID);
    glBindVertexArray(this->vaoID);
        glGenBuffers(1, &this->vboID);
        glBindBuffer(GL_ARRAY_BUFFER, this->vboID);
            int verticesSize = this->vertices.size() * sizeof(float);
            int normalsSize = this->normals.size() * sizeof(float);
            int colorsSize = this->colors.size() * sizeof(float);

            // Allocate to the GPU
            glBufferData(GL_ARRAY_BUFFER, verticesSize + normalsSize + colorsSize, 0, GL_STATIC_DRAW);

            glBufferSubData(GL_ARRAY_BUFFER, 0,                          verticesSize, &this->vertices[0]);
            glBufferSubData(GL_ARRAY_BUFFER, verticesSize,               normalsSize,   &this->normals[0]);
            glBufferSubData(GL_ARRAY_BUFFER, verticesSize + normalsSize, colorsSize,  &this->colors[0]);

            // vertices
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);

            // normals
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(verticesSize));
            glEnableVertexAttribArray(1);

            // colors
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(verticesSize + normalsSize));
            glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::render()
{
    glBindVertexArray(this->vaoID);
    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() / 3);
    glBindVertexArray(0);
}
