#pragma once
#include "client/rendering/Renderer.h"
#include "client/rendering/assets/AssetManager.h"
#include "client/rendering/mesh/Mesh.h"
#include "common/ecs/System.h"
#include "common/world/World.h"
#include "../mesh/BlockVertex.h"
#include "../mesh/LayeredMeshData.h"


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
