#pragma once
#include "common/ecs/System.h"
#include "common/world/World.h"

namespace Mineclone {
    class ChunkLoadingSystem : public System {
    public:
        ChunkLoadingSystem(World& world, int simulationDistance);

        void update(Registry &registry, float dt) override;
        void setSimulationDistance(const int simulationDistance) {
            m_simulationDistance = simulationDistance;
        }
    private:
        World& m_world;
        int m_simulationDistance;
    };
}
