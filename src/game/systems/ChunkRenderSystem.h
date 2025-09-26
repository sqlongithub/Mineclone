#pragma once
#include "ecs/System.h"
#include "game/world/World.h"
#include "game/world/mesher/BlockVertex.h"
#include "game/world/mesher/LayeredMeshData.h"
#include "renderer/Renderer.h"
#include "renderer/assets/AssetManager.h"
#include "renderer/mesh/Mesh.h"
#include "renderer/render_queue/RenderLayer.h"

namespace Mineclone {
    class ChunkRenderSystem : public System {
    public:
        ChunkRenderSystem(World& world, AssetManager& assets, BlockRegistry& blockRegistry, BiomeRegistry& biomeRegistry);

        void update(Registry &registry, float dt) override;

        static RawMeshData convertToRawMesh(const LayeredMeshData<BlockVertex> &layeredData);

        void draw(Renderer& renderer, Registry &registry, float dt) override;
    private:
        AssetManager& m_assets;
        BlockRegistry& m_blockRegistry;
        BiomeRegistry& m_biomeRegistry;
        World& m_world;
        // cpu
        std::unordered_map<ChunkPos, LayeredMeshData<BlockVertex>, ChunkPosHash> m_chunkMeshData;
        // gpu
        std::unordered_map<ChunkPos, Mesh, ChunkPosHash> m_chunkMeshes;


    };

}
