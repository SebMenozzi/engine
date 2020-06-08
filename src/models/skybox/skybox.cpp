#include "skybox.h"

#ifndef BUFFER_OFFSET
  #define BUFFER_OFFSET(offset) ((char*) 0 + offset)
#endif

Skybox::Skybox(float size, std::vector<std::string> faces)
: Cube(size), cubemap(faces)
{
    this->cubemap.load();
}

void Skybox::load()
{
    if(glIsBuffer(this->vboID) == GL_TRUE)
        glDeleteBuffers(1, &this->vboID);

    glGenVertexArrays(1, &this->vaoID);
    glBindVertexArray(this->vaoID);
        glGenBuffers(1, &this->vboID);
        glBindBuffer(GL_ARRAY_BUFFER, this->vboID);
            int verticesSize = this->vertices.size() * sizeof(float);

            // Allocate to the GPU
            glBufferData(GL_ARRAY_BUFFER, verticesSize, 0, GL_STATIC_DRAW);

            glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, &this->vertices[0]);

            // vertices
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Skybox::render()
{
    glFrontFace(GL_CW);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemap.getID());

    glBindVertexArray(this->vaoID);
    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() / 3);
    glDepthMask(GL_TRUE);

    glFrontFace(GL_CCW);
}
