#include "PhysicsMovementSystem.h"
#include "common/ecs/components/Transform.h"
#include <glm/glm.hpp>
#include <algorithm>
#include <cmath>

namespace Mineclone {

    PhysicsMovementSystem::PhysicsMovementSystem(World& world)
        : m_world(world) {}

    void PhysicsMovementSystem::processMovement(Velocity& velocity, PlayerState& playerState,
                                                const MoveIntent& intent,
                                                const PhysicsProperties& props, float dt) {
        bool grounded = playerState.isGrounded;

        // ===========================
        // Flying movement
        // ===========================
        if (playerState.isFlying) {
            glm::vec2 horizInput(intent.direction.x, intent.direction.z);
            float horizLen = glm::length(horizInput);
            if (horizLen > 1e-6f) horizInput /= horizLen;

            float flySpeed = props.flySpeed * (intent.sprint ? 2.0f : 1.0f);
            glm::vec2 targetHoriz = horizInput * flySpeed;
            float targetVert = 0.0f;
            if (intent.jump) targetVert = flySpeed;
            if (intent.crouch) targetVert = -flySpeed;

            float flyAccel = 200.0f;

            glm::vec2 currentHoriz(velocity.velocity.x, velocity.velocity.z);
            glm::vec2 horizDiff = targetHoriz - currentHoriz;
            float horizDiffLen = glm::length(horizDiff);
            if (horizDiffLen > 0.001f) {
                float maxChange = flyAccel * dt;
                if (horizDiffLen > maxChange) horizDiff *= maxChange / horizDiffLen;
                velocity.velocity.x += horizDiff.x;
                velocity.velocity.z += horizDiff.y;
            }

            float vertDiff = targetVert - velocity.velocity.y;
            float maxVertChange = flyAccel * dt;
            if (std::abs(vertDiff) > maxVertChange)
                vertDiff = (vertDiff > 0.0f ? maxVertChange : -maxVertChange);
            velocity.velocity.y += vertDiff;

            // Apply frame-rate independent drag using exponential decay
            float dragMultiplier = std::exp(-props.airDragRate * dt);

            velocity.velocity.x *= dragMultiplier;
            velocity.velocity.z *= dragMultiplier;

            if (props.verticalDragRate > 0.0f) {
                velocity.velocity.y *= std::exp(-props.verticalDragRate * dt);
            }

            return;
        }

        // ===========================
        // Ground / walking movement
        // ===========================
        glm::vec2 input(intent.direction.x, intent.direction.z);
        float inputLen = glm::length(input);
        if (inputLen > 1e-6f) input /= inputLen;

        float moveSpeed = props.walkSpeed * (intent.sprint ? props.sprintScale : 1.0f);
        glm::vec2 targetVel = input * moveSpeed;
        glm::vec2 currentHoriz(velocity.velocity.x, velocity.velocity.z);

        float accel = grounded ? props.groundAcceleration : props.airAcceleration;

        glm::vec2 velocityDiff = targetVel - currentHoriz;
        float diffLen = glm::length(velocityDiff);
        if (diffLen > 0.001f) {
            float maxChange = accel * dt;
            if (diffLen > maxChange) velocityDiff *= maxChange / diffLen;
            velocity.velocity.x += velocityDiff.x;
            velocity.velocity.z += velocityDiff.y;
        }

        // Jump - instant velocity like Minecraft (0.42 blocks/tick = 8.4 blocks/sec)
        if (intent.jump && grounded) {
            velocity.velocity.y = props.jumpVelocity;

            // Sprint jump boost (Minecraft adds 0.2 blocks/tick = 4.0 blocks/sec)
            if (intent.sprint && inputLen > 0.01f) {
                velocity.velocity.x += input.x * 4.0f;
                velocity.velocity.z += input.y * 4.0f;
            }
            playerState.isGrounded = false;
        }

        // Gravity
        velocity.velocity.y += props.gravity * dt;
        velocity.velocity.y = std::max(velocity.velocity.y, props.terminalVelocity);

        // ===========================
        // Frame-rate independent drag using exponential decay
        // ===========================
        float horizontalDragRate = grounded ? props.groundDragRate : props.airDragRate;
        float dragMultiplier = std::exp(-horizontalDragRate * dt);

        velocity.velocity.x *= dragMultiplier;
        velocity.velocity.z *= dragMultiplier;

        if (props.verticalDragRate > 0.0f) {
            velocity.velocity.y *= std::exp(-props.verticalDragRate * dt);
        }

        // Small velocity cutoff
        if (std::abs(velocity.velocity.x) < 0.003f) velocity.velocity.x = 0.0f;
        if (std::abs(velocity.velocity.y) < 0.003f) velocity.velocity.y = 0.0f;
        if (std::abs(velocity.velocity.z) < 0.003f) velocity.velocity.z = 0.0f;
    }

    void PhysicsMovementSystem::update(Registry& registry, float dt) {
        const auto& props = registry.getSingleton<PhysicsProperties>();

        registry.view<Transform, Velocity, PlayerState, MoveIntent>(
            [&](Entity, Transform&, Velocity& v, PlayerState& ps, MoveIntent& intent) {
                if (!ps.hasCollision()) return;
                processMovement(v, ps, intent, props, dt);
            });
    }

}