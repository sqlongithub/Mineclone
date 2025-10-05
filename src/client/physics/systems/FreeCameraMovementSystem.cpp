#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>

#include "client/input/InputAction.h"
#include "client/input/components/InputState.h"
#include "client/rendering/components/Camera.h"
#include "common/ecs/Registry.h"
#include "common/ecs/components/Transform.h"
#include "common/world/World.h"

#include "FreeCameraMovementSystem.h"

#include "common/physics/components/MoveIntent.h"
#include "common/player/components/Player.h"

namespace Mineclone {
    FreeCameraMovementSystem::FreeCameraMovementSystem(World& world)
        : m_world(world) {
        std::cout << "Free camera movemeent system initializaed\n";

    }

    void FreeCameraMovementSystem::update(Registry &registry, float dt) {
        const auto& input = registry.getSingleton<InputState>();

        registry.view<Transform, PlayerState, MoveIntent>([&](Entity e, Transform& transform, PlayerState& playerState, MoveIntent& intent) {
            if (playerState.gameMode != GameMode::NOCLIP) return;

            float speed = 14.0f;
            if (intent.sprint) speed *= 2;

            glm::vec3 movement = intent.direction;

            if (intent.jump) movement.y += 1.0f;
            if (intent.crouch) movement.y -= 1.0f;

            if (glm::length(movement) > 0.001f) {
                movement = glm::normalize(movement);
            }

            transform.position += movement * speed * dt;
        });
    }
}
