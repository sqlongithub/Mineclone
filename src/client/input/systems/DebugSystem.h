#pragma once
#include "common/ecs/System.h"


namespace Mineclone {
    class DebugSystem : public System {
    public:
        DebugSystem();
        void update(Mineclone::Registry &registry, float dt) override;
    private:
    };

}
