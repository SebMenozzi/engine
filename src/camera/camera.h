#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../input/input.h"

class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 targetPoint, glm::vec3 verticalAxis, float sensitivity, float velocity);
    ~Camera();

    void move(Input const &input);
    void lookAt(glm::mat4 &view);
    void setPosition(glm::vec3 position);
    glm::vec3 getPosition();
    float getSensitivity() const;
    float getVelocity() const;
    void setSensitivity(float sensitivity);
    void setVelocity(float velocity);

private:
    float phi;
    float theta;
    glm::vec3 orientation;

    glm::vec3 verticalAxis;
    glm::vec3 lateralMovement;

    glm::vec3 position;
    glm::vec3 targetPoint;

    float sensitivity;
    float velocity;

    void orientate(int xRel, int yRel);
    void setTargetPoint(glm::vec3 targetPoint);
};
