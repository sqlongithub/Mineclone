#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/vec3.hpp"

#include <glm/gtx/quaternion.hpp>

#include "glm/fwd.hpp"

namespace Mineclone {
    struct Transform {
        glm::vec3 position = glm::vec3(0.0f);        // World position
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);  // Orientation as quaternion
        glm::vec3 scale    = glm::vec3(1.0f);        // Scale along each axis

        [[nodiscard]] glm::mat4 getMatrix() const {
            glm::mat4 mat = glm::translate(glm::mat4(1.0f), position);
            mat *= glm::toMat4(rotation);
            mat = glm::scale(mat, scale);
            return mat;
        }

        [[nodiscard]] glm::vec3 getForwardVector() const {
            return rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        }

        [[nodiscard]] glm::vec3 getRightVector() const {
            return rotation * glm::vec3(1.0f, 0.0f, 0.0f);
        }
    };
}