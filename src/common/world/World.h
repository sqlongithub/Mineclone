#pragma once

#include <unordered_map>
#include "Chunk.h"
#include "glm/vec3.hpp"
#include "TerrainGenerator.h"
#include "WorldPos.h"

namespace Mineclone {

    struct RaycastHit {
        glm::ivec3 blockPos;
        glm::vec3 hitPoint;
        const Block& block;
        CubeFace face;
    };

    class World {
    public:
        World(BlockRegistry& blockRegistry, BiomeRegistry& biomeRegistry, int seed);

        Chunk& getChunk(ChunkPos pos);
        [[nodiscard]] const Chunk* getChunk(ChunkPos pos) const;

        Block* getBlock(WorldPos pos);
        const Block* getBlock(WorldPos pos) const;
        BiomeId getBiome(WorldPos pos) const;

        void setBlock(WorldPos pos, const Block& block);
        void setBlock(WorldPos pos, BlockId id) {
            setBlock(pos, Block(id));
        }

        std::optional<RaycastHit> raycast(glm::vec3 origin, glm::vec3 dir) const;

        const std::unordered_map<ChunkPos, Chunk, ChunkPosHash>& getLoadedChunks() const;
        std::unordered_map<ChunkPos, Chunk, ChunkPosHash>& getLoadedChunks();
        void loadChunksAroundPosition(WorldPos pos, int radius = 2);

        void unloadChunksOutsideRadius(WorldPos center, int radius);

        void unloadChunk(ChunkPos pos);

        int getSurfaceHeight(int worldX, int worldZ);

    private:
        BlockRegistry& m_blockRegistry;
        BiomeRegistry& m_biomeRegistry;
        TerrainGenerator m_terrainGenerator;
        std::unordered_map<ChunkPos, Chunk, ChunkPosHash> m_loadedChunks;
    };

}
