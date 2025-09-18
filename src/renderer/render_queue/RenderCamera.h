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
        glm::vec3 up;
        glm::vec3 right;

        float nearPlane;
        float farPlane;

        RenderCamera(const glm::mat4& proj, const glm::mat4& v, const glm::vec3& pos,
                     float near, float far)
                : projection(proj), view(v), position(pos), nearPlane(near), farPlane(far) {
            viewProjection = projection * view;

            forward = -glm::vec3(view[0][2], view[1][2], view[2][2]);
            up = glm::vec3(view[0][1], view[1][1], view[2][1]);
            right = glm::vec3(view[0][0], view[1][0], view[2][0]);
        }
    };
}