#pragma once

#include <glm/glm.hpp>

namespace material
{
    class Material
    {
        public:
            Material(const glm::vec3& ka, const glm::vec3& kd, const glm::vec3& ks)
                : ka_(ka), kd_(kd), ks_(ks)
            {}

            glm::vec3 ka() const { return ka_; }
            glm::vec3 kd() const { return kd_; }
            glm::vec3 ks() const { return ks_; }

        private:
            glm::vec3 ka_; // Ambient
            glm::vec3 kd_; // Diffuse
            glm::vec3 ks_; // Specular
    };
}