#include "ChunkRenderSystem.h"
#include "world/ChunkMesher.h"
#include "render_queue/RenderCommand.h"
#include "AssetManager.h"
#include "Renderer.h"

namespace Mineclone {

    ChunkRenderSystem::ChunkRenderSystem(World& world, AssetManager& assets, BlockRegistry& blockRegistry)
        : m_assets(assets), m_blockRegistry(blockRegistry), m_world(world) {

    }

    void ChunkRenderSystem::update(Registry &registry, float dt) {
        auto& loadedChunks = m_world.getLoadedChunks();

        for (auto it = m_chunkMeshes.begin(); it != m_chunkMeshes.end(); ) {
            if (loadedChunks.find(it->first) == loadedChunks.end()) {
                // unloaded chunk
                it = m_chunkMeshes.erase(it);
            } else {
                ++it;
            }
        }

        for (auto& [pos, chunk] : loadedChunks) {
            if (!chunk.isMeshDirty()) continue;

            m_chunkMeshes.insert_or_assign(
                    pos,
                    ChunkMesher::buildMesh(m_assets.blockTextures(), m_blockRegistry, chunk)
            );

            Mesh& mesh = m_chunkMeshes.find(pos)->second;

            std::cout << "Generated mesh ";


            chunk.markMeshClean();
        }
    }

    void ChunkRenderSystem::draw(Renderer& renderer, Registry& registry, float dt) {
        for(auto& [pos, mesh] : m_chunkMeshes) {
            RenderCommand cmd {
                &mesh,
                m_assets.materials().getHandle("texture"),
                pos.getModelMatrix(),
                Layer::TERRAIN
            };
            renderer.submit(cmd);
        }
    }
}