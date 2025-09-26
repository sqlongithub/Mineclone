#pragma once
#include "ecs/Registry.h"
#include "ecs/System.h"
#include "game/world/World.h"

namespace Mineclone {
    class FreeCameraMovementSystem : public System {
    public:
        explicit FreeCameraMovementSystem(World& world);

        void update(Registry& registry, float dt) override;
    private:
        World& m_world;
    };
}
