#pragma once

typedef unsigned char uint8;
typedef signed char sint8;
typedef unsigned short uint16;
typedef signed short sint16;
typedef unsigned long uint32;
typedef signed long sint32;

typedef float float16;
typedef double double32;

typedef struct
{
    int x, y, z;
} vertex;

enum facePosition { ABOVE, BELOW, LEFT, RIGHT, FRONT, BACK };

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef struct
{
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
} DirLight;

typedef struct
{
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
} PointLight;

typedef struct
{
    glm::vec3 position;
    glm::vec3 direction;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
} SpotLight;
