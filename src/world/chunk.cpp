#include "chunk.h"

#define CHUNK_SIZE 1.0

#ifndef BUFFER_OFFSET
    #define BUFFER_OFFSET(offset) ((char*) 0 + offset)
#endif

Chunk::Chunk(int size, int height, const char* vertexPath, const char* fragmentPath)
    : data(size * CHUNK_SIZE, height * CHUNK_SIZE, size * CHUNK_SIZE),
      shader(vertexPath, fragmentPath)
{
    this->shader.load();
    this->size = size;
    this->height = height;

    this->vaoID = 0;
    this->vboID = 0;

    this->position.x = 0;
    this->position.y = 0;
    this->position.z = 0;
}

Chunk::~Chunk()
{
    glDeleteVertexArrays(1, &this->vaoID);
    glDeleteBuffers(1, &this->vboID);
}

void Chunk::setBlock(int x, int y, int z, int value)
{
    if (outOfBound(x, y, z))
        return;

    data(x, y, z) = value;
}

int Chunk::getBlock(int x, int y, int z)
{
    if (outOfBound(x, y, z))
        return 0;

    return data(x, y, z);
}

bool Chunk::outOfBound(int x, int y, int z)
{
    if (x >= (this->size * CHUNK_SIZE)) return true;
    if (y >= (this->height * CHUNK_SIZE)) return true;
    if (z >= (this->size * CHUNK_SIZE)) return true;

    if (x < 0) return true;
    if (y < 0) return true;
    if (z < 0) return true;

    return false;
}

void Chunk::setPosition(glm::vec3 position)
{
    this->position = position;
}

void Chunk::fillRandom()
{
    srand(time(NULL));

    // Fill Chunk data with random value
    for (int x = 0; x < this->size; x += CHUNK_SIZE)
    {
        for (int y = 0; y < this->height; y += CHUNK_SIZE)
        {
            for (int z = 0; z < this->size; z += CHUNK_SIZE)
            {
                int value = rand() % 2;
                setBlock(x, y, z, value);
            }
        }
    }

    for (int x = 0; x < this->size; x += CHUNK_SIZE)
    {
        for (int y = 0; y < this->height; y += CHUNK_SIZE)
        {
            for (int z = 0; z < this->size; z += CHUNK_SIZE)
            {

                int neightbor1 = getBlock(x - CHUNK_SIZE, y, z);
                int neightbor2 = getBlock(x + CHUNK_SIZE, y, z);
                int neightbor3 = getBlock(x, y - CHUNK_SIZE, z);
                int neightbor4 = getBlock(x, y + CHUNK_SIZE, z);
                int neightbor5 = getBlock(x, y, z - CHUNK_SIZE);
                int neightbor6 = getBlock(x, y, z + CHUNK_SIZE);

                int value = getBlock(x, y, z);

                // if the current block is solid
                if (value == 1)
                {
                    if (neightbor1 == 0) addFace(x, y, z, LEFT);
                    if (neightbor2 == 0) addFace(x, y, z, RIGHT);
                    if (neightbor3 == 0) addFace(x, y, z, BELOW);
                    if (neightbor4 == 0) addFace(x, y, z, ABOVE);
                    if (neightbor5 == 0) addFace(x, y, z, BACK);
                    if (neightbor6 == 0) addFace(x, y, z, FRONT);
                }
            }
        }
    }
}

/*
              ABOVE      FRONT
                  |     /
              2-------0
             /|      /|
            / |     / |
           /  |    /  |
 LEFT --  3---|---1   | -- RIGHT
          |   5---|---4
          |  /    |  /
          | /     | /
          |/      |/
   (x,y,z)7-------6
         /      |
     BACK       BELOW

          |<----->|
            size
*/
void Chunk::addFace(int x, int y, int z, facePosition facing)
{
    vertex v;

    if (facing == ABOVE)
    {
        // vertices
        v.x = x + CHUNK_SIZE; v.y = y + CHUNK_SIZE; v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 0
        v.x = x + CHUNK_SIZE; v.y = y + CHUNK_SIZE; v.z = z;              this->vertices.push_back(v); // 1
        v.x = x;              v.y = y + CHUNK_SIZE; v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 2

        v.x = x;              v.y = y + CHUNK_SIZE; v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 2
        v.x = x + CHUNK_SIZE; v.y = y + CHUNK_SIZE; v.z = z;              this->vertices.push_back(v); // 1
        v.x = x;              v.y = y + CHUNK_SIZE; v.z = z;              this->vertices.push_back(v); // 3

        // colors
        v.x = 1.0; v.y = 0.0; v.z = 0.0; this->colors.push_back(v); // 0
        v.x = 1.0; v.y = 0.0; v.z = 0.0; this->colors.push_back(v); // 1
        v.x = 1.0; v.y = 0.0; v.z = 0.0; this->colors.push_back(v); // 2

        v.x = 1.0; v.y = 0.0; v.z = 0.0; this->colors.push_back(v); // 2
        v.x = 1.0; v.y = 0.0; v.z = 0.0; this->colors.push_back(v); // 1
        v.x = 1.0; v.y = 0.0; v.z = 0.0; this->colors.push_back(v); // 3
    }
    else if (facing == BELOW)
    {
        // vertices
        v.x = x + CHUNK_SIZE; v.y = y;              v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 4
        v.x = x;              v.y = y;              v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 5
        v.x = x + CHUNK_SIZE; v.y = y;              v.z = z;              this->vertices.push_back(v); // 6

        v.x = x + CHUNK_SIZE; v.y = y;              v.z = z;              this->vertices.push_back(v); // 6
        v.x = x;              v.y = y;              v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 5
        v.x = x;              v.y = y;              v.z = z;              this->vertices.push_back(v); // 7

        // colors
        v.x = 1.0; v.y = 1.0; v.z = 0.0; this->colors.push_back(v); // 4
        v.x = 1.0; v.y = 1.0; v.z = 0.0; this->colors.push_back(v); // 5
        v.x = 1.0; v.y = 1.0; v.z = 0.0; this->colors.push_back(v); // 6

        v.x = 1.0; v.y = 1.0; v.z = 0.0; this->colors.push_back(v); // 6
        v.x = 1.0; v.y = 1.0; v.z = 0.0; this->colors.push_back(v); // 5
        v.x = 1.0; v.y = 1.0; v.z = 0.0; this->colors.push_back(v); // 7
    }
    else if (facing == LEFT)
    {
        // vetices
        v.x = x;              v.y = y + CHUNK_SIZE; v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 2
        v.x = x;              v.y = y + CHUNK_SIZE; v.z = z;              this->vertices.push_back(v); // 3
        v.x = x;              v.y = y;              v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 5

        v.x = x;              v.y = y;              v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 5
        v.x = x;              v.y = y + CHUNK_SIZE; v.z = z;              this->vertices.push_back(v); // 3
        v.x = x;              v.y = y;              v.z = z;              this->vertices.push_back(v); // 7

        // colors
        v.x = 0.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 2
        v.x = 0.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 3
        v.x = 0.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 5

        v.x = 0.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 5
        v.x = 0.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 3
        v.x = 0.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 7
    }
    else if (facing == RIGHT)
    {
        // vertices
        v.x = x + CHUNK_SIZE; v.y = y + CHUNK_SIZE; v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 0
        v.x = x + CHUNK_SIZE; v.y = y;              v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 4
        v.x = x + CHUNK_SIZE; v.y = y + CHUNK_SIZE; v.z = z;              this->vertices.push_back(v); // 1

        v.x = x + CHUNK_SIZE; v.y = y + CHUNK_SIZE; v.z = z;              this->vertices.push_back(v); // 1
        v.x = x + CHUNK_SIZE; v.y = y;              v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 4
        v.x = x + CHUNK_SIZE; v.y = y;              v.z = z;              this->vertices.push_back(v); // 6

        // colors
        v.x = 1.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 0
        v.x = 1.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 4
        v.x = 1.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 1

        v.x = 1.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 1
        v.x = 1.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 4
        v.x = 1.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 6
    }
    else if (facing == FRONT)
    {
        // vertices
        v.x = x + CHUNK_SIZE; v.y = y + CHUNK_SIZE; v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 0
        v.x = x;              v.y = y + CHUNK_SIZE; v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 2
        v.x = x;              v.y = y;              v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 5

        v.x = x;              v.y = y;              v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 5
        v.x = x + CHUNK_SIZE; v.y = y;              v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 4
        v.x = x + CHUNK_SIZE; v.y = y + CHUNK_SIZE; v.z = z + CHUNK_SIZE; this->vertices.push_back(v); // 0

        // colors
        v.x = 0.0; v.y = 1.0; v.z = 0.0; this->colors.push_back(v); // 0
        v.x = 0.0; v.y = 1.0; v.z = 0.0; this->colors.push_back(v); // 2
        v.x = 0.0; v.y = 1.0; v.z = 0.0; this->colors.push_back(v); // 5

        v.x = 0.0; v.y = 1.0; v.z = 0.0; this->colors.push_back(v); // 5
        v.x = 0.0; v.y = 1.0; v.z = 0.0; this->colors.push_back(v); // 4
        v.x = 0.0; v.y = 1.0; v.z = 0.0; this->colors.push_back(v); // 0
    }
    else if (facing == BACK)
    {
        // vertices
        v.x = x;              v.y = y;              v.z = z;              this->vertices.push_back(v); // 7
        v.x = x;              v.y = y + CHUNK_SIZE; v.z = z;              this->vertices.push_back(v); // 3
        v.x = x + CHUNK_SIZE; v.y = y + CHUNK_SIZE; v.z = z;              this->vertices.push_back(v); // 1

        v.x = x + CHUNK_SIZE; v.y = y + CHUNK_SIZE; v.z = z;              this->vertices.push_back(v); // 1
        v.x = x + CHUNK_SIZE; v.y = y;              v.z = z;              this->vertices.push_back(v); // 6
        v.x = x;              v.y = y;              v.z = z;              this->vertices.push_back(v); // 7

        // colors
        v.x = 0.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 7
        v.x = 0.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 3
        v.x = 0.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 1

        v.x = 0.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 1
        v.x = 0.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 6
        v.x = 0.0; v.y = 0.0; v.z = 1.0; this->colors.push_back(v); // 7
    }
}

void Chunk::load()
{
    if (glIsBuffer(this->vboID) == GL_TRUE)
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

void Chunk::display(glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model)
{
    GLuint worldPositionID = glGetUniformLocation(this->shader.getProgramID(), "worldPosition");
    GLuint viewID = glGetUniformLocation(this->shader.getProgramID(), "view");
    GLuint modelID = glGetUniformLocation(this->shader.getProgramID(), "model");
    GLuint projectionID = glGetUniformLocation(this->shader.getProgramID(), "projection");
    //GLuint normalID = glGetUniformLocation(this->shader.getProgramID(), "normal");

    glUseProgram(this->shader.getProgramID());
        glUniform3fv(worldPositionID, 1, glm::value_ptr(this->position));
        glUniformMatrix4fv(viewID, 1, GL_FALSE, value_ptr(view));
        glUniformMatrix4fv(modelID, 1, GL_FALSE, value_ptr(model));
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(projection));
        //glUniform3fv(normalID, 1, glm::value_ptr(glm::vec3(0, 0, 1)));

        glBindVertexArray(this->vaoID);
        glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
    glUseProgram(0);
}
