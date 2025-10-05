#include "ChunkLoadingSystem.h"

#include "common/ecs/components/Transform.h"
#include "common/player/components/Player.h"

#include <unordered_set>

namespace Mineclone {
    ChunkLoadingSystem::ChunkLoadingSystem(World &world, int simulationDistance)
        : m_world(world), m_simulationDistance(simulationDistance) {

    }

    void ChunkLoadingSystem::update(Registry &registry, float dt) {
        std::unordered_set<ChunkPos, ChunkPosHash> chunksToKeep;


        registry.view<PlayerIdentity, Transform>([&](Entity e, PlayerIdentity&, Transform& transform) {
            auto playerChunk = ChunkPos::fromWorldPos(transform.position);

            for (int x = -m_simulationDistance; x <= m_simulationDistance; x++) {
                for (int z = -m_simulationDistance; z <= m_simulationDistance; z++) {
                    ChunkPos chunkPos = {playerChunk.x + x, playerChunk.z + z};
                    chunksToKeep.insert(chunkPos);
                    m_world.getChunk(chunkPos); // load if not exists
                }
            }
        });


        for (auto it = m_world.getLoadedChunks().begin(); it != m_world.getLoadedChunks().end();) {
            if (chunksToKeep.find(it->first) == chunksToKeep.end()) {
                it = m_world.getLoadedChunks().erase(it);
            } else {
                ++it;
            }
        }
    }

}
