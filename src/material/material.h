#pragma once

#include <glm/glm.hpp>

namespace material
{
    class Material
    {
        public:
            Material(const glm::vec4& ka, const glm::vec4& kd, const glm::vec4& ks)
                : ka_(ka), kd_(kd), ks_(ks)
            {}

            glm::vec3 get_ka() const { return ka_; }
            glm::vec3 get_kd() const { return kd_; }
            glm::vec3 get_ks() const { return ks_; }

            void set_ka(const glm::vec4& ka) { ka_ = ka; }
            void set_kd(const glm::vec4& kd) { kd_ = kd; }
            void set_ks(const glm::vec4& ks) { ks_ = ks; }

        private:
            glm::vec3 ka_; // Ambient
            glm::vec3 kd_; // Diffuse
            glm::vec3 ks_; // Specular
    };
}