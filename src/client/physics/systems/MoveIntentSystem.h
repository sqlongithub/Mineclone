#pragma once
#include "common/ecs/System.h"


namespace Mineclone {
    class MoveIntentSystem : public System {
    public:
        explicit MoveIntentSystem() = default;
        void update(Registry &registry, float dt) override;
        void draw(Renderer &renderer, Registry &registry, float dt) override;
    private:
        Entity m_lastEntity;
    };
}
