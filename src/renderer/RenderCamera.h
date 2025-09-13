#pragma once
#include "glm/vec3.hpp"
#include "glm/fwd.hpp"
#include "glm/detail/type_mat4x4.hpp"

namespace Mineclone {
    struct RenderCamera {
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 viewProjection;
        glm::vec3 position;
        glm::vec3 forward;

        RenderCamera(const glm::mat4& proj, const glm::mat4& v, const glm::vec3& pos)
                : projection(proj), view(v), position(pos) {
            viewProjection = projection * view;
            forward = -glm::vec3(view[0][2], view[1][2], view[2][2]);  // Extract forward vector
        }
    };
}