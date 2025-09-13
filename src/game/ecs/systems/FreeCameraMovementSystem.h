#pragma once

#include "ecs/ISystem.h"
#include "Window.h"

namespace Mineclone {
    class FreeCameraMovementSystem : public ISystem {
    public:
        explicit FreeCameraMovementSystem(Dispatcher& dispatcher);


        void update(Registry& registry, float dt) override;
    private:

    };
}
