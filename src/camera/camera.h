#pragma once

#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtx/transform.hpp>

#include "maths_utils.h"
#include "window.h"

namespace camera
{
    class Camera
    {
        public:
            Camera(glm::vec3 position, glm::vec3 targetPoint, glm::vec3 verticalAxis, float sensitivity, float velocity);
            ~Camera();

            void move(window::Window const &window);
            void lookAt(glm::mat4 &view);
            void setPosition(glm::vec3 position);
            glm::vec3 getPosition();
            float getSensitivity() const;
            float getVelocity() const;
            void setSensitivity(float sensitivity);
            void setVelocity(float velocity);

        private:
            float horizontalAngle_;
            float verticalAngle_;
            glm::vec3 direction_;

            glm::vec3 verticalAxis_;
            glm::vec3 lateralDirection_;

            glm::vec3 position_;
            glm::vec3 targetPosition_;

            float sensitivity_;
            float velocity_;

            void init_();
            void orientate_(int xRel, int yRel);
    };
}