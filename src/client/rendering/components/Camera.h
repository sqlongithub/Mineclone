#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "common/ecs/components/Transform.h"
#include "client/rendering/render_queue/RenderCamera.h"

namespace Mineclone {

    enum class ProjectionType {
        Perspective,
        Orthographic
    };

    struct Camera {
        ProjectionType projection = ProjectionType::Perspective;
        float nearPlane = 0.05f;
        float farPlane  = 1000.0f;
        float orthoLeft   = -10.0f;
        float orthoRight  = 10.0f;
        float orthoBottom = -10.0f;
        float orthoTop    = 10.0f;

        [[nodiscard]] glm::mat4 getProjectionMatrix(float verticalFOV, float windowWidth, float windowHeight) const {
            float aspect = windowWidth / windowHeight;
            if (projection == ProjectionType::Perspective) {
                return glm::perspective(verticalFOV, aspect, nearPlane, farPlane);
            }
            return glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, nearPlane, farPlane);
        }

        [[nodiscard]] glm::mat4 getViewMatrix(const Transform& t, float eyeHeight) const {
            glm::vec3 eyePos = t.position + glm::vec3(0.0f, eyeHeight, 0.0f);
            glm::vec3 forward = t.rotation * glm::vec3(0, 0, -1);
            glm::vec3 up      = t.rotation * glm::vec3(0, 1, 0);
            return glm::lookAt(eyePos, eyePos + forward, up);
        }

        [[nodiscard]] RenderCamera toRenderCamera(
            const Transform& t,
            float verticalFOV,
            float eyeHeight,
            float windowWidth,
            float windowHeight
        ) const {
            glm::mat4 proj = getProjectionMatrix(verticalFOV, windowWidth, windowHeight);
            glm::mat4 view = getViewMatrix(t, eyeHeight);
            return {
                proj,
                view,
                t.position + glm::vec3(0.0f, eyeHeight, 0.0f),
                nearPlane,
                farPlane
            };
        }
    };

}
