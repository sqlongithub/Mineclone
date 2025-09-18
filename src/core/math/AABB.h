#pragma once

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

namespace Mineclone {
    struct AABB {
        glm::vec3 min;
        glm::vec3 max;

        AABB()
            : min(std::numeric_limits<float>::max()),
              max(std::numeric_limits<float>::lowest()) {}

        AABB(const glm::vec3& min, const glm::vec3& max)
                : min(min), max(max) {}

        [[nodiscard]] glm::vec3 center() const { return (min + max) * 0.5f; }
        [[nodiscard]] glm::vec3 size() const { return max - min; }

        void expand(const glm::vec3& point) {
            min = glm::min(min, point);
            max = glm::max(max, point);
        }

        [[nodiscard]] AABB transformed(const glm::mat4& model) const {
            glm::vec3 center = (min + max) * 0.5f;
            glm::vec3 extent = (max - min) * 0.5f;

            glm::vec3 worldCenter = glm::vec3(model * glm::vec4(center, 1.0f));

            auto rotScale = glm::mat3(model);

            glm::vec3 worldExtent;
            worldExtent.x = glm::dot(glm::abs(rotScale[0]), extent);
            worldExtent.y = glm::dot(glm::abs(rotScale[1]), extent);
            worldExtent.z = glm::dot(glm::abs(rotScale[2]), extent);

            return AABB{worldCenter - worldExtent, worldCenter + worldExtent};
        }


        float distanceToPoint(const glm::vec3& point) {
            glm::vec3 closest;
            closest.x = std::clamp(point.x, min.x, max.x);
            closest.y = std::clamp(point.y, min.y, max.y);
            closest.z = std::clamp(point.z, min.z, max.z);

            return glm::length(closest - point);
        }

    };
}
