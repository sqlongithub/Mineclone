
#include "CameraControlSystem.h"

#include "client/input/components/InputState.h"
#include "client/rendering/components/Camera.h"
#include "client/rendering/components/CameraController.h"
#include "common/ecs/components/Transform.h"
#include "common/player/components/Player.h"

namespace Mineclone {
    void CameraControlSystem::update(Registry &registry, float dt) {
        auto& input = registry.getSingleton<InputState>();

        registry.view<CameraController, Transform, PlayerState>([&](Entity e, CameraController& controller, Transform& transform, PlayerState& playerState) {
            if(input.isCursorCaptured) {
                controller.yaw -= input.getMouseDeltaX() * controller.lookSensitivity;
                controller.pitch -= input.getMouseDeltaY() * controller.lookSensitivity;
                controller.pitch = glm::clamp(controller.pitch, -89.0f, 89.0f);

                glm::quat qPitch = glm::angleAxis(glm::radians(controller.pitch), glm::vec3(1,0,0));
                glm::quat qYaw = glm::angleAxis(glm::radians(controller.yaw), glm::vec3(0,1,0));
                transform.rotation = glm::normalize(qYaw * qPitch);
            }

            float targetFov = controller.baseFov * (playerState.isSprinting ? controller.sprintFovScale : 1.0f);
            controller.fov += (targetFov - controller.fov) * std::clamp(dt * controller.fovLerpSpeed, 0.0f, 1.0f);

        });
    }

}
