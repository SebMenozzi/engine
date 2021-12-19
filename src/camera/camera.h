#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "utils/maths_utils.h"
#include "input.h"

namespace camera
{
    class Camera
    {
        public:
            Camera(glm::vec3 position, glm::vec3 targetPoint, glm::vec3 verticalAxis, float sensitivity, float velocity);
            ~Camera();

            void move(input::Input const &input);
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