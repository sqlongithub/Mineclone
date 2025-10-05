#pragma once
#include "common/ecs/Registry.h"
#include "common/ecs/System.h"
#include "common/world/World.h"
#include "common/world/BlockRegistry.h"
#include "common/physics/components/PhysicsProperties.h"
#include "common/physics/components/Collider.h"
#include "common/ecs/components/Transform.h"
#include "common/ecs/components/Velocity.h"
#include "common/player/components/Player.h"

namespace Mineclone {

    class CollisionSystem : public System {
    public:
        CollisionSystem(World& world, BlockRegistry& blockRegistry);

        void update(Registry& registry, float dt) override;

    private:
        World& m_world;
        BlockRegistry& m_blockRegistry;

        float computeAllowedMovement(const glm::vec3& pos, const Collider& collider,
                                     float movement, int axis);

        bool tryStepUp(Transform& transform, const Collider& collider,
                       float reqX, float reqZ, float stepHeight);

        void applyFriction(Velocity& velocity, float frictionAccel, float dt);

        void processEntity(Entity e, Transform& transform, Velocity& velocity,
                           Collider& collider, PlayerState* pstate,
                           const PhysicsProperties& props, float dt);
    };

}
