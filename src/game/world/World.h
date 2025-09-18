#pragma once

#include <unordered_map>
#include "Chunk.h"
#include "glm/vec3.hpp"
#include "TerrainGenerator.h"

namespace Mineclone {
    struct WorldPos {
        int x, y, z;

        explicit WorldPos(int x, int y, int z)
            : x(x), y(y), z(z) {}

        WorldPos(glm::vec3 pos)
            : x(pos.x), y(pos.y), z(pos.z) {}

        [[nodiscard]] ChunkPos toChunkPos() const {
            return {x >> 4, z >> 4}; // divide by 16
        }

        struct LocalPos {
            int x, y, z; // within chunk (0-16)
        };

        [[nodiscard]] LocalPos toLocalPos() const {
            return {x & 15, y, z & 15};
        }
    };

    class World {
    public:
        World(BlockRegistry& blockRegistry, int seed);

        Chunk& getChunk(ChunkPos pos);

        const Block& getBlock(WorldPos pos) const;
        void setBlock(WorldPos pos, const Block& block);
        void setBlock(WorldPos pos, BlockId id) {
            setBlock(pos, Block(id));
        }

        const std::unordered_map<ChunkPos, Chunk, ChunkPosHash>& getLoadedChunks() const;
        std::unordered_map<ChunkPos, Chunk, ChunkPosHash>& getLoadedChunks();
        void loadChunksAroundPosition(WorldPos pos, int radius = 2);

        int getSurfaceHeight(int worldX, int worldZ);

    private:
        BlockRegistry& m_blockRegistry;
        TerrainGenerator m_terrainGenerator;
        std::unordered_map<ChunkPos, Chunk, ChunkPosHash> m_loadedChunks;
    };

}
