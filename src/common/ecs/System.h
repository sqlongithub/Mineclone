#pragma once

#include "Registry.h"
#include "client/rendering/Renderer.h"


namespace Mineclone {
    class System {
    public:
        virtual ~System() = default;
        virtual void update([[maybe_unused]] Registry& registry, [[maybe_unused]] float dt) {};
        virtual void draw(Renderer& renderer, [[maybe_unused]] Registry& registry, [[maybe_unused]] float dt) {};
    };
}
