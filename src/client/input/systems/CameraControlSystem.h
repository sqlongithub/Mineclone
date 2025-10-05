#pragma once
#include "common/ecs/System.h"


namespace Mineclone {
    class CameraControlSystem : public System {
    public:
        CameraControlSystem() = default;

        void update(Registry &registry, float dt) override;
    private:
    };
}