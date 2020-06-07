#include "skybox.h"

#ifndef BUFFER_OFFSET
  #define BUFFER_OFFSET(offset) ((char*) 0 + offset)
#endif

Skybox::Skybox(float size, const char* vertexPath, const char* fragmentPath, std::vector<std::string> faces)
: Cube(size, vertexPath, fragmentPath), cubemap(faces)
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

void Skybox::display(glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model)
{
    GLuint worldPositionID = glGetUniformLocation(this->shader.getProgramID(), "worldPosition");
    GLuint projectionID = glGetUniformLocation(this->shader.getProgramID(), "projection");
    GLuint viewID = glGetUniformLocation(this->shader.getProgramID(), "view");
    GLuint modelID = glGetUniformLocation(this->shader.getProgramID(), "model");

    glUseProgram(this->shader.getProgramID());
        glFrontFace(GL_CW);

        glUniform3fv(worldPositionID, 1, glm::value_ptr(this->position));
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(model));

        glUniform1i(glGetUniformLocation(this->shader.getProgramID(), "skybox"), 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemap.getID());

        glBindVertexArray(this->vaoID);
        glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() / 3);
        glDepthMask(GL_TRUE);

        glFrontFace(GL_CCW);
    glUseProgram(0);
}
