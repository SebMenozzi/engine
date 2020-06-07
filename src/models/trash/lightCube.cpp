#include "lightCube.h"

LightCube::LightCube(float size, const char* vertexPath, const char* fragmentPath, glm::vec3 color) : Cube(size, vertexPath, fragmentPath)
{
  this->color = color;
}

LightCube::~LightCube()
{

}

void LightCube::display(glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model)
{
  GLuint worldPositionID = glGetUniformLocation(this->shader.getProgramID(), "worldPosition");
  GLuint projectionID = glGetUniformLocation(this->shader.getProgramID(), "projection");
  GLuint viewID = glGetUniformLocation(this->shader.getProgramID(), "view");
  GLuint modelID = glGetUniformLocation(this->shader.getProgramID(), "model");
  GLuint objectColorID = glGetUniformLocation(this->shader.getProgramID(), "objectColor");
  GLuint lightColorID = glGetUniformLocation(this->shader.getProgramID(), "lightColor");

  glUseProgram(this->shader.getProgramID());
    glUniform3fv(worldPositionID, 1, glm::value_ptr(this->position));
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(model));

    glUniform3fv(objectColorID, 1, glm::value_ptr(this->color));
    glUniform3fv(lightColorID, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));

    glBindVertexArray(this->vaoID);
    glDrawArrays(GL_TRIANGLES, 0, 36);

  glUseProgram(0);
}
