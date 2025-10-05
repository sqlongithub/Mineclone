#pragma once
#include "common/ecs/System.h"

namespace Mineclone {
    class PlayerAbilitiesSystem : public System {
    public:
        PlayerAbilitiesSystem() = default;

        void update(Registry &registry, float dt) override;
    private:
        std::unordered_map<Entity, float> m_lastJumpTime;
        std::unordered_map<Entity, bool> m_wasJumping;
        float m_totalTime = 0.0f;
    };
}