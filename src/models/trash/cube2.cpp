#include "cube.h"

#ifndef BUFFER_OFFSET
  #define BUFFER_OFFSET(offset) ((char*) 0 + offset)
#endif

Cube::Cube(float size, const char* vertexPath, const char* fragmentPath): shader(vertexPath, fragmentPath)
{
  this->shader.loadShaders();

  //this->vaoID = 0;
  this->vboID = 0;
  this->eboID = 0;
  this->verticesSizeBytes = 24 * sizeof(float);
  this->colorsSizeBytes = 9 * sizeof(float);
  this->indicesSizeBytes = 36 * sizeof(unsigned int);

  // Avoid getting a double sized cube
  size /= 2;

  float verticesTmp[] = {
    -size, -size, -size, // 0
    size, -size, -size, // 1
    size, size, -size, // 2
    -size, size, -size, // 3
    size, -size, size, // 4
    size, size, size, // 5
    -size, -size, size, // 6
    -size, size, size // 7
  };

  for(int i = 0; i < 24; ++i) {
    this->vertices[i] = verticesTmp[i];
  }

  unsigned int vertexIndicesTmp[] = {
    // Side 1
    0, 1, 2,  // Triangle 1
    0, 3, 2,  // Triangle 2
    // Side 2
    4, 1, 2,  // Triangle 3
    4, 5, 2,  // Triangle 4
    // Side 3
    6, 4, 1, // Triangle 5
    6, 0, 1, // Triangle 6
    // Side 4
    6, 4, 5,   // Triangle 7
    6, 7, 5,   // Triangle 8
    // Side 5
    0, 6, 7,  // Triangle 9
    0, 3, 7,  // Triangle 10
    // Side 6
    7, 5, 2,   // Triangle 11
    7, 3, 2   // Triangle 12
  };

  for(int i = 0; i < 36; ++i) {
    this->vertexIndices[i] = vertexIndicesTmp[i];
  }

  float colorsTmp[] = {
    1.0, 0.0, 0.0, // red
    0.0, 1.0, 0.0, // green
    0.0, 0.0, 1.0 // blue
  };

  for(int i = 0; i < 9; ++i) {
    this->colors[i] = colorsTmp[i];
  }

  // Temporary colors
  unsigned int colorIndicesTmp[] = {
    // Side 1
    0, 0, 0, // Triangle 1
    0, 0, 0, // Triangle 2
    // Side 2
    1, 1, 1, // Triangle 3
    1, 1, 1, // Triangle 4
    // Side 3
    2, 2, 2, // Triangle 5
    2, 2, 2, // Triangle 6
    // Side 4
    0, 0, 0, // Triangle 7
    0, 0, 0, // Triangle 8
    // Side 5
    1, 1, 1, // Triangle 9
    1, 1, 1, // Triangle 10
    // Side 6
    2, 2, 2, // Triangle 11
    2, 2, 2  // Triangle 12
  };

  for(int i = 0; i < 36; ++i) {
    this->colorIndices[i] = colorIndicesTmp[i];
  }
}

Cube::~Cube()
{
  //glDeleteVertexArrays(1, &this->vaoID);
  glDeleteBuffers(1, &this->vboID);
  glDeleteBuffers(1, &this->eboID);
}

void Cube::load()
{
  if(glIsBuffer(this->vboID) == GL_TRUE) {
    glDeleteBuffers(1, &this->vboID);
  }

  if(glIsBuffer(this->eboID) == GL_TRUE) {
    glDeleteBuffers(1, &this->eboID);
  }

  //glGenVertexArrays(1, &this->vaoID);
  glGenBuffers(1, &this->vboID);
  glGenBuffers(1, &this->eboID);

  //glBindVertexArray(this->vaoID);

  glBindBuffer(GL_ARRAY_BUFFER, this->vboID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
  //glBufferData(GL_ARRAY_BUFFER, this->verticesSizeBytes + this->colorsSizeBytes, 0, GL_STATIC_DRAW);
  //glBufferSubData(GL_ARRAY_BUFFER, 0, this->verticesSizeBytes, this->vertices);
  //glBufferSubData(GL_ARRAY_BUFFER, this->verticesSizeBytes, this->colorsSizeBytes, this->colors);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->vertexIndices), this->vertexIndices, GL_STATIC_DRAW);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indicesSizeBytes * 2, 0, GL_STATIC_DRAW);
  //glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, this->indicesSizeBytes, this->vertexIndices);
  //glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, this->indicesSizeBytes, this->indicesSizeBytes, this->colorIndices);

  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  //glEnableVertexAttribArray(0);

  //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(this->verticesSizeBytes));
  //glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind vbo is allowed

   // note: do not ubind EBO while VAO is active, keep the EBO bound.
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void Cube::display(glm::mat4 &projection, glm::mat4 &modelView)
{
  GLuint modelViewID = glGetUniformLocation(this->shader.getProgramID(), "modelView");
  GLuint projectionID = glGetUniformLocation(this->shader.getProgramID(), "projection");

  glUseProgram(this->shader.getProgramID());

    glUniformMatrix4fv(modelViewID, 1, GL_FALSE, value_ptr(modelView));
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, value_ptr(projection));

    glBindBuffer(GL_ARRAY_BUFFER, this->vboID);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //glBindVertexArray(this->vaoID);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // no need to unbind it every time
    //glBindVertexArray(0);

  glUseProgram(0);
}
