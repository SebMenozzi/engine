#include "uvSphere.h"

#ifndef BUFFER_OFFSET
    #define BUFFER_OFFSET(offset) ((char*) 0 + offset)
#endif

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT  = 2;

// from http://www.songho.ca/opengl/gl_sphere.html

UVSphere::UVSphere(float radius, int sectorCount, int stackCount)
    : Mesh()
{
    this->eboID = 0;

    this->radius = radius / 2;
    this->sectorCount = (sectorCount < MIN_SECTOR_COUNT) ? MIN_SECTOR_COUNT : sectorCount;
    this->stackCount = (stackCount < MIN_STACK_COUNT) ? MIN_STACK_COUNT : stackCount;
}

UVSphere::~UVSphere()
{
    glDeleteVertexArrays(1, &this->vaoID);
    glDeleteBuffers(1, &this->vboID);
    glDeleteBuffers(1, &this->eboID);
}

void UVSphere::load()
{
    // Clear memory
    std::vector<float>().swap(this->vertices);
    std::vector<float>().swap(this->normals);
    std::vector<float>().swap(this->textureCoord);
    std::vector<unsigned int>().swap(this->indices);

    // generate vertices, normals and texture coords

    const float PI = 3.14159265359;

    float lengthInv = 1.0f / this->radius;
    float sectorStep = 2 * PI / this->sectorCount;
    float stackStep = PI / this->stackCount;

    for (int i = 0; i <= this->stackCount; ++i)
    {
        float stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
        float xy = this->radius * cosf(stackAngle); // r * cos(u)
        float z = this->radius * sinf(stackAngle);  // r * sin(u)

        for (int j = 0; j <= this->sectorCount; ++j)
        {
            float sectorAngle = j * sectorStep; // starting from 0 to 2pi

            // vertices
            float x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            float y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)

            this->vertices.push_back(x);
            this->vertices.push_back(y);
            this->vertices.push_back(z);

            // normals
            float nx = x * lengthInv;
            float ny = y * lengthInv;
            float nz = z * lengthInv;

            this->normals.push_back(nx);
            this->normals.push_back(ny);
            this->normals.push_back(nz);

            // texture coords
            float s = (float) j / this->sectorCount;
            float t = (float) i / this->stackCount;
            this->textureCoord.push_back(s);
            this->textureCoord.push_back(t);
        }
    }

    // generate indices

    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1

    for (int i = 0; i < this->stackCount; ++i)
    {

        int k1 = i * (sectorCount + 1); // beginning of current stack
        int k2 = k1 + sectorCount + 1;  // beginning of next stack

        for (int j = 0; j < this->sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks

            // k1 => k2 => k1 + 1
            if (i != 0)
            {
                this->indices.push_back(k1);
                this->indices.push_back(k2);
                this->indices.push_back(k1 + 1);
            }

            // k1 + 1 => k2 => k2 + 1
            if (i != (this->stackCount - 1))
            {
                this->indices.push_back(k1 + 1);
                this->indices.push_back(k2);
                this->indices.push_back(k2 + 1);
            }
        }
    }

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

        glGenBuffers(1, &this->eboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

        // vertices
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(0);

        // normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(verticesSize));
        glEnableVertexAttribArray(1);

        // textureCoord
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(verticesSize + normalsSize));
        glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void UVSphere::render()
{
    glBindVertexArray(this->vaoID);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*) 0);
    glBindVertexArray(0);
}
