#pragma once
#include "common/ecs/Registry.h"
#include "common/world/World.h"
#include "common/physics/components/PhysicsProperties.h"
#include "common/player/components/Player.h"
#include "common/physics/components/MoveIntent.h"
#include "common/ecs/components/Velocity.h"
#include <glm/glm.hpp>

#include "common/ecs/System.h"

namespace Mineclone {

    class PhysicsMovementSystem : public System {
    public:
        explicit PhysicsMovementSystem(World& world);

        void update(Registry& registry, float dt);

    private:
        World& m_world;

        void processMovement(Velocity& velocity, PlayerState& playerState,
                             const MoveIntent& intent, const PhysicsProperties& props, float dt);

        void applyAcceleration(glm::vec2& horizontal, const glm::vec2& desired,
                               float accel, float dt);
    };

}
