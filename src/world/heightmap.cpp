#include "heightmap.h"

#ifndef BUFFER_OFFSET
  #define BUFFER_OFFSET(offset) ((char*) 0 + offset)
#endif

Heightmap::Heightmap(int size, float step, const char* vertexPath, const char* fragmentPath)
    : shader(vertexPath, fragmentPath)
{
    this->shader.loadShaders();

    this->size = size;
    this->step = step;

    this->vaoID = 0;
    this->vboID = 0;

    std::vector<int> tmp;
    for (int x = 0; x < this->size; ++x)
    {
        tmp.clear();
        for (int z = 0; z < this->size; ++z)
            tmp.push_back(0);

        this->heights.push_back(tmp);
    }
}

Heightmap::~Heightmap()
{
    glDeleteVertexArrays(1, &this->vaoID);
    glDeleteBuffers(1, &this->vboID);
}

bool Heightmap::outOfBound(int x, int z)
{
    if (x >= this->size) return true;
    if (z >= this->size) return true;

    if (x < 0) return true;
    if (z < 0) return true;

    return false;
}

void Heightmap::circlesAlgorithm(int iterations)
{
  for (int i = 0; i < iterations; ++i) {
    //create a circle at an x and y with radius between 1 and 20 using random numbers
    int randomX = rand() % this->size; //randomly select x center point for circle
    int randomZ = rand()% this->size; //randomly select z center point for circle
    int randomRadius = (rand() % 20) + 1;  //randomly generate radius integer between 1 - 20 to be the terrainCircleSize
    float disp = (rand() % 12) + 1; //randomly generate a value of disp between 1 - 12
    //loop through the entire height map and run the circle algorithm to check if a given x and y is within the circle and
    //if so apply the algorithm to it to modify the height to the new value
    for (int x = 0; x < this->size; ++x) {
      for (int z = 0; z < this->size; ++z){
        int dx = x - randomX; //get the value for tx to determine distance from circle center
        int dz = z - randomZ;  //get the value for ty to determine distance from circle center
        float distance = sqrtf((dx * dx) + (dz * dz)); //get the distance from circle center
        float pd = (distance * 2) / randomRadius; //get the value for pd
        if (fabs(pd) <= 1.0) { //check if value is within the circle
          this->heights[x][z] += (disp / 2.0) + (cos(pd * 3.14) * (disp / 2.0)); //apply circle alorithm to get the new height
        }
      }
    }
  }

  for (int x = 0; x < this->size; ++x) {
    for (int z = 0; z < this->size; ++z){
      addTile(x, z);
    }
  }
}

void Heightmap::findMinMaxHeight()
{
    for (int x = 0; x < this->size; ++x)
    {
        for (int z = 0; z< this->size; ++z)
        {
            if (this->heights[x][z] < minHeight)
                this->minHeight = this->heights[x][z];
            else if (this->heights[x][z] > maxHeight)
                this->maxHeight = this->heights[x][z];
        }
    }
}

int Heightmap::getHeight(int x, int z)
{
    if (outOfBound(x, z))
        return 0;

    return this->heights[x][z];
}

void Heightmap::addTile(int x, int z)
{
    vertex v;

    int h0 = getHeight(x, z);
    int h1 = getHeight(x + 1, z);
    int h2 = getHeight(x, z + 1);
    int h3 = getHeight(x + 1, z + 1);

    std::cout << h0 << h1 << h2 << h3 << std::endl;

    v.x = x;     v.y = h0;   v.z = z;     this->vertices.push_back(v); // 0
    v.x = x + 1; v.y = h1;   v.z = z;     this->vertices.push_back(v); // 1
    v.x = x;     v.y = h2;   v.z = z + 1; this->vertices.push_back(v); // 2

    v.x = x;     v.y = h2;   v.z = z + 1; this->vertices.push_back(v); // 2
    v.x = x + 1; v.y = h3;   v.z = z + 1; this->vertices.push_back(v); // 3
    v.x = x + 1; v.y = h1;   v.z = z;     this->vertices.push_back(v); // 1

    v.x = 1; v.y = 0; v.z = 0; // red
    for (int i = 0; i < 6; ++i) this->colors.push_back(v);
}

void Heightmap::load()
{
    if(glIsBuffer(this->vboID) == GL_TRUE)
        glDeleteBuffers(1, &this->vboID);

    glGenVertexArrays(1, &this->vaoID);
    glBindVertexArray(this->vaoID);
        glGenBuffers(1, &this->vboID);
        glBindBuffer(GL_ARRAY_BUFFER, this->vboID);
            int verticesSize = this->vertices.size() * sizeof(vertex);
            int colorsSize = this->colors.size() * sizeof(vertex);

            glBufferData(GL_ARRAY_BUFFER, verticesSize + colorsSize, 0, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, &this->vertices[0]);
            glBufferSubData(GL_ARRAY_BUFFER, verticesSize, colorsSize, &this->colors[0]);

            glVertexAttribPointer(0, 3, GL_UNSIGNED_INT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_UNSIGNED_INT, GL_FALSE, 0, BUFFER_OFFSET(verticesSize));
            glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Heightmap::display(glm::mat4 &projection, glm::mat4 &modelView)
{
    GLuint modelViewID = glGetUniformLocation(this->shader.getProgramID(), "modelView");
    GLuint projectionID = glGetUniformLocation(this->shader.getProgramID(), "projection");

    glUseProgram(this->shader.getProgramID());
        glUniformMatrix4fv(modelViewID, 1, GL_FALSE, value_ptr(modelView));
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, value_ptr(projection));

        glBindVertexArray(this->vaoID);
        glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
    glUseProgram(0);
}
