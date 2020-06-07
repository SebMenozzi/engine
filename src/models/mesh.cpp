#include "mesh.h"

#ifndef BUFFER_OFFSET
  #define BUFFER_OFFSET(offset) ((char*) 0 + offset)
#endif

Mesh::Mesh(float size, const char* vertexPath, const char* fragmentPath): shader(vertexPath, fragmentPath)
{
    this->shader.loadShaders();

    this->vaoID = 0;
    this->vboID = 0;

    this->position.x = 0;
    this->position.y = 0;
    this->position.z = 0;
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

void Mesh::display(glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model)
{
    GLuint worldPositionID = glGetUniformLocation(this->shader.getProgramID(), "worldPosition");
    GLuint projectionID = glGetUniformLocation(this->shader.getProgramID(), "projection");
    GLuint viewID = glGetUniformLocation(this->shader.getProgramID(), "view");
    GLuint modelID = glGetUniformLocation(this->shader.getProgramID(), "model");

    glUseProgram(this->shader.getProgramID());
        glUniform3fv(worldPositionID, 1, glm::value_ptr(this->position));
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, value_ptr(projection));
        glUniformMatrix4fv(viewID, 1, GL_FALSE, value_ptr(view));
        glUniformMatrix4fv(modelID, 1, GL_FALSE, value_ptr(model));

        glBindVertexArray(this->vaoID);
        glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() / 3);
    glUseProgram(0);
}

void Mesh::setPosition(glm::vec3 position)
{
    this->position = position;
}
