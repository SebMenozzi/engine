#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../input/input.h"

class Camera
{
public:
    /**
     * @brief Define the Camera object
     * @param position, targetPoint, verticalAxis, sensitivity, velocity
     * @return void
    */
    Camera(glm::vec3 position, glm::vec3 targetPoint, glm::vec3 verticalAxis, float sensitivity, float velocity);
    ~Camera();

    /**
     * @brief Move the Camera according the Input object
     * @param input
     * @return void
    */
    void move(Input const &input);

    /**
     * @brief Orientate the Camera according a view matrix
     * @param view
     * @return void
    */
    void lookAt(glm::mat4 &view);

    /**
     * @brief Define the position of the Camera
     * @param position
     * @return void
    */
    void setPosition(glm::vec3 position);

    /**
     * @brief Retrieve the Camera position
     * @param void
     * @return position
    */
    glm::vec3 getPosition();

    /**
     * @brief Retrieve the Camera sensitivity
     * @param void
     * @return sensitivity
    */
    float getSensitivity() const;

    /**
     * @brief Retrieve the Camera velocity
     * @param void
     * @return velocity
    */
    float getVelocity() const;

    /**
     * @brief Set the Camera sensitivity
     * @param sensitivity
     * @return void
    */
    void setSensitivity(float sensitivity);

    /**
     * @brief Set the Camera velocity
     * @param velocity
     * @return void
    */
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
