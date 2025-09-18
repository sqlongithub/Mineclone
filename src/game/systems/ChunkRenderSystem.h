#pragma once

#include "System.h"
#include "World.h"
#include "mesh/Mesh.h"
#include "AssetManager.h"
#include "Renderer.h"

namespace Mineclone {
    class ChunkRenderSystem : public System {
    public:
        ChunkRenderSystem(World& world, AssetManager& assets, BlockRegistry& blockRegistry);

        void update(Registry &registry, float dt) override;
        void draw(Renderer& renderer, Registry &registry, float dt) override;
    private:
        AssetManager& m_assets;
        BlockRegistry& m_blockRegistry;
        World& m_world;
        std::unordered_map<ChunkPos, Mesh, ChunkPosHash> m_chunkMeshes;


    };

}
