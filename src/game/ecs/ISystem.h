#pragma once

#include "ecs/Registry.h"

namespace Mineclone {
    class ISystem {
    public:
        virtual ~ISystem() = default;
        virtual void update(Registry& registry, float dt) {};
        virtual void draw(Registry& registry, float dt) {};
    };
}