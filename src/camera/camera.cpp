#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 targetPoint, glm::vec3 verticalAxis, float sensitivity, float velocity)
{
    this->phi = 0.0;
    this->theta = 0.0;
    this->position = position;
    this->targetPoint = targetPoint;
    this->verticalAxis = verticalAxis;

    this->sensitivity = sensitivity;
    this->velocity = velocity;

    setTargetPoint(targetPoint);
}

Camera::~Camera()
{

}

void Camera::orientate(int xRel, int yRel)
{
    this->phi += -yRel * this->sensitivity;
    this->theta += -xRel * this->sensitivity;

    // Limit the phi angle
    if(this->phi > 89.0)
        this->phi = 89.0;
    else if(this->phi < -89.0)
        this->phi = -89.0;

    // Conversion to radian
    float phiRadian = this->phi * M_PI / 180;
    float thetaRadian = this->theta * M_PI / 180;

    if (this->verticalAxis.x == 1.0)
    {
        this->orientation.x = sin(phiRadian);
        this->orientation.y = cos(phiRadian) * cos(thetaRadian);
        this->orientation.z = cos(phiRadian) * sin(thetaRadian);
    }
    else if(this->verticalAxis.y == 1.0)
    {
        this->orientation.x = cos(phiRadian) * sin(thetaRadian);
        this->orientation.y = sin(phiRadian);
        this->orientation.z = cos(phiRadian) * cos(thetaRadian);
    }
    else
    {
        this->orientation.x = cos(phiRadian) * cos(thetaRadian);
        this->orientation.y = cos(phiRadian) * sin(thetaRadian);
        this->orientation.z = sin(phiRadian);
    }

    this->lateralMovement = glm::cross(this->verticalAxis, this->orientation);
    this->lateralMovement = glm::normalize(this->lateralMovement);

    this->targetPoint = this->position + this->orientation;
}

void Camera::move(Input const &input)
{
    if (input.isCursorMoving())
        orientate(input.getXRel(), input.getYRel());

    // forward
    if (input.getKey(SDL_SCANCODE_UP) || input.getKey(SDL_SCANCODE_W))
    {
        this->position = this->position + this->orientation * this->velocity;
        this->targetPoint = this->position + this->orientation;
    }

    // backward
    if (input.getKey(SDL_SCANCODE_DOWN) || input.getKey(SDL_SCANCODE_S))
    {
        this->position = this->position - this->orientation * this->velocity;
        this->targetPoint = this->position + this->orientation;
    }

    // left
    if(input.getKey(SDL_SCANCODE_LEFT) || input.getKey(SDL_SCANCODE_A))
    {
        this->position = this->position + this->lateralMovement * this->velocity;
        this->targetPoint = this->position + this->orientation;
    }

    // right
    if(input.getKey(SDL_SCANCODE_RIGHT) || input.getKey(SDL_SCANCODE_D))
    {
        this->position = this->position - this->lateralMovement * this->velocity;
        this->targetPoint = this->position + this->orientation;
    }
}

void Camera::lookAt(glm::mat4 &view)
{
    view = glm::lookAt(this->position, this->targetPoint, this->verticalAxis);
}

void Camera::setTargetPoint(glm::vec3 targetPoint)
{
    this->orientation = this->targetPoint - this->position;
    this->orientation = glm::normalize(this->orientation);

    if(this->verticalAxis.x == 1.0)
    {
        this->phi = asin(this->orientation.x);
        this->theta = acos(this->orientation.y / cos(this->phi));

        if (this->orientation.y < 0)
            this->theta *= -1;
    }
    else if(this->verticalAxis.y == 1.0)
    {
        this->phi = asin(this->orientation.y);
        this->theta = acos(this->orientation.z / cos(this->phi));

        if (this->orientation.z < 0)
            this->theta *= -1;
    }
    else
    {
        this->phi = asin(this->orientation.x);
        this->theta = acos(this->orientation.z / cos(this->phi));

        if (this->orientation.z < 0)
            this->theta *= -1;
    }

    // Conversion to degree
    this->phi = this->phi * 180 / M_PI;
    this->theta = this->theta * 180 / M_PI;
}

void Camera::setPosition(glm::vec3 position)
{
  this->position = position;
  this->targetPoint = this->position + this->orientation;
}

glm::vec3 Camera::getPosition()
{
  return this->position;
}

float Camera::getSensitivity() const
{
  return this->sensitivity;
}

float Camera::getVelocity() const
{
  return this->velocity;
}

void Camera::setSensitivity(float sensitivity)
{
  this->sensitivity = sensitivity;
}

void Camera::setVelocity(float velocity)
{
  this->velocity = velocity;
}
