#pragma once

#include "Window.h"
#include "World.h"
#include "System.h"
#include "Registry.h"

namespace Mineclone {
    class FreeCameraMovementSystem : public System {
    public:
        explicit FreeCameraMovementSystem(World& world);

        void update(Registry& registry, float dt) override;
    private:
        World& m_world;
    };
}
