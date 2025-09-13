
#include "FreeCameraMovementSystem.h"
#include "ecs/components/InputData.h"
#include "ecs/components/Transform.h"
#include "Camera.h"
#include "input/InputAction.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace Mineclone {
    FreeCameraMovementSystem::FreeCameraMovementSystem(Dispatcher& dispatcher) {
        std::cout << "Free camera movemeent system initializaed\n";
    }

    void FreeCameraMovementSystem::update(Registry &registry, float dt) {
        auto& input = registry.getSingleton<InputState>();

        // Query entities with Transform
        registry.view<Transform, Camera>([&](Entity e, Transform& transform, Camera& camera) {

            glm::vec3 forward = transform.getForwardVector();
            glm::vec3 right = transform.getRightVector();
            float speed = 5.0f; // units per second

            if (input.isActionPressed(InputAction::MOVE_FORWARD)) {
                transform.position += forward * speed * dt;
            }
            if (input.isActionPressed(InputAction::MOVE_BACKWARD)) {
                transform.position -= forward * speed * dt;
            }
            if (input.isActionPressed(InputAction::MOVE_LEFT)) {
                transform.position -= right * speed * dt;
            }
            if (input.isActionPressed(InputAction::MOVE_RIGHT)) {
                transform.position += right * speed * dt;
            }
            if (input.isActionPressed(InputAction::JUMP)) {
                std::cout << "jump?\n";
                transform.position.y += speed * dt; // simplistic
            }

            float mouseSensitivity = 0.05f;
            camera.yaw   -= input.getMouseDeltaX() * mouseSensitivity;
            camera.pitch -= input.getMouseDeltaY() * mouseSensitivity;
            camera.pitch = glm::clamp(camera.pitch, -89.0f, 89.0f);

            glm::quat qPitch = glm::angleAxis(glm::radians(camera.pitch), glm::vec3(1,0,0));
            glm::quat qYaw   = glm::angleAxis(glm::radians(camera.yaw), glm::vec3(0,1,0));
            transform.rotation = glm::normalize(qYaw * qPitch);
        });
    }
}