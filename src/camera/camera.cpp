#include "camera.h"

namespace camera
{
    // MARK: - Public

    Camera::Camera(
        glm::vec3 position,
        glm::vec3 targetPosition,
        glm::vec3 verticalAxis,
        float sensitivity,
        float velocity
    ): 
        horizontalAngle_(0.0),
        verticalAngle_(0.0),
        verticalAxis_(verticalAxis),
        position_(position),
        targetPosition_(targetPosition),
        sensitivity_(sensitivity),
        velocity_(velocity)
    {
        init_();
    }

    Camera::~Camera() {}

    void Camera::move(window::Window const &window)
    {
        if (window.isCursorMoving())
            orientate_(window.getXRel(), window.getYRel());

        // Forward
        if (window.isKeyPressed(GLFW_KEY_UP) || window.isKeyPressed(GLFW_KEY_W))
            position_ = position_ + direction_ * velocity_;

        // Backward
        if (window.isKeyPressed(GLFW_KEY_DOWN) || window.isKeyPressed(GLFW_KEY_S))
            position_ = position_ - direction_ * velocity_;

        // Left
        if (window.isKeyPressed(GLFW_KEY_LEFT) || window.isKeyPressed(GLFW_KEY_A))
            position_ = position_ + lateralDirection_ * velocity_;

        // Right
        if (window.isKeyPressed(GLFW_KEY_RIGHT) || window.isKeyPressed(GLFW_KEY_D))
            position_ = position_ - lateralDirection_ * velocity_;

        targetPosition_ = position_ + direction_;
    }

    void Camera::lookAt(glm::mat4 &view)
    {
        view = glm::lookAt(position_, targetPosition_, verticalAxis_);
    }

    void Camera::setPosition(glm::vec3 position) {
        position_ = position;
        targetPosition_ = position_ + direction_;
    }

    glm::vec3 Camera::getPosition()
    {
        return position_;
    }

    float Camera::getSensitivity() const
    {
        return sensitivity_;
    }

    float Camera::getVelocity() const
    {
        return velocity_;
    }

    void Camera::setSensitivity(float sensitivity)
    {
        sensitivity_ = sensitivity;
    }

    void Camera::setVelocity(float velocity)
    {
        velocity_ = velocity;
    }

    // MARK: - Private

    void Camera::orientate_(int xRel, int yRel)
    {
        verticalAngle_ += -yRel * sensitivity_;
        horizontalAngle_ += -xRel * sensitivity_;

        // Limit the angle of the y axis
        verticalAngle_ = utils::clamp(verticalAngle_, -89, 89);

        float verticalAngleRadian = glm::radians(verticalAngle_);
        float horizontalAngleRadian = glm::radians(horizontalAngle_);

        if (verticalAxis_.x == 1.0)
        {
            direction_.x = sin(verticalAngleRadian);
            direction_.y = cos(verticalAngleRadian) * cos(horizontalAngleRadian);
            direction_.z = cos(verticalAngleRadian) * sin(horizontalAngleRadian);
        }
        else if(verticalAxis_.y == 1.0)
        {
            direction_.x = cos(verticalAngleRadian) * sin(horizontalAngleRadian);
            direction_.y = sin(verticalAngleRadian);
            direction_.z = cos(verticalAngleRadian) * cos(horizontalAngleRadian);
        } 
        else
        {
            direction_.x = cos(verticalAngleRadian) * cos(horizontalAngleRadian);
            direction_.y = cos(verticalAngleRadian) * sin(horizontalAngleRadian);
            direction_.z = sin(verticalAngleRadian);
        }

        lateralDirection_ = glm::normalize(
            glm::cross(verticalAxis_, direction_)
        );

        targetPosition_ = position_ + direction_;
    }

    void Camera::init_()
    {
        // Normalize vector betwen the postion and the target position
        direction_ = glm::normalize(targetPosition_ - position_);

        if (verticalAxis_.x == 1.0)
        {
            verticalAngle_ = asin(direction_.x);
            horizontalAngle_ = acos(direction_.y / cos(verticalAngle_));

            if (direction_.y < 0)
                horizontalAngle_ *= -1;
        }
        else if(verticalAxis_.y == 1.0)
        {
            verticalAngle_ = asin(direction_.y);
            horizontalAngle_ = acos(direction_.z / cos(verticalAngle_));

            if (direction_.z < 0)
                horizontalAngle_ *= -1;
        } 
        else
        {
            verticalAngle_ = asin(direction_.x);
            horizontalAngle_ = acos(direction_.z / cos(verticalAngle_));

            if (direction_.z < 0)
                horizontalAngle_ *= -1;
        }

        verticalAngle_ = glm::degrees(verticalAngle_);
        horizontalAngle_ = glm::radians(horizontalAngle_);
    }
}