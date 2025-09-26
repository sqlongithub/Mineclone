#pragma once
#include "Transform.h"
#include <glm/glm.hpp>

namespace Mineclone {

    enum class ProjectionType {
        Perspective,
        Orthographic
    };

    struct Camera {
        ProjectionType projection = ProjectionType::Perspective;

        float fovY = 45.0f;
        float aspect = 16.0f / 9.0f;
        float nearPlane = 0.1f;
        float farPlane  = 1000.0f;

        float orthoLeft   = -10.0f;
        float orthoRight  = 10.0f;
        float orthoBottom = -10.0f;
        float orthoTop    = 10.0f;

        float yaw = 0.0f;
        float pitch = 0.0f;

        [[nodiscard]] glm::mat4 getProjectionMatrix() const {
            if (projection == ProjectionType::Perspective) {
                return glm::perspective(glm::radians(fovY), aspect, nearPlane, farPlane);
            } else {
                return glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, nearPlane, farPlane);
            }
        }

        [[nodiscard]] glm::mat4 getViewMatrix(const Transform& t) const {
            glm::quat orientation = glm::normalize(
                    glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) *
                    glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0))
            );

            glm::vec3 forward = orientation * glm::vec3(0, 0, -1); // OpenGL camera looks -Z by default
            glm::vec3 up      = orientation * glm::vec3(0, 1, 0);

            return glm::lookAt(t.position, t.position + forward, up);
        }

        [[nodiscard]] glm::vec3 getForwardVector() const {
            glm::quat orientation = glm::normalize(
                    glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) *
                    glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0))
            );
            return orientation * glm::vec3(0, 0, -1);
        }

        [[nodiscard]] glm::vec3 getRightVector() const {
            glm::quat orientation = glm::normalize(
                    glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) *
                    glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0))
            );
            return orientation * glm::vec3(1, 0, 0);
        }

        [[nodiscard]] glm::vec3 getUpVector() const {
            glm::quat orientation = glm::normalize(
                    glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) *
                    glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0))
            );
            return orientation * glm::vec3(0, 1, 0);
        }

        [[nodiscard]] RenderCamera toRenderCamera(const Transform& transform) const {
            glm::mat4 proj = getProjectionMatrix();
            glm::mat4 view = getViewMatrix(transform);

            return { proj, view, transform.position, nearPlane, farPlane };
        }


    };

}
