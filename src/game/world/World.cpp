#include "World.h"
#include "ChunkPos.h"

namespace Mineclone {
    World::World(BlockRegistry& blockRegistry, int seed)
        : m_blockRegistry(blockRegistry), m_terrainGenerator(seed) {

    }

    Chunk& World::getChunk(ChunkPos pos) {
        auto it = m_loadedChunks.find(pos);
        if (it != m_loadedChunks.end()) {
            return it->second; // Return existing chunk
        }

        // Chunk does not exist — create and insert
        auto& chunk = m_loadedChunks.emplace(pos, Chunk()).first->second;
        m_terrainGenerator.generate(pos, chunk, m_blockRegistry);
        return chunk;
    }

    const Block& World::getBlock(WorldPos pos) const {
        ChunkPos chunkPos = pos.toChunkPos();
        auto localPos = pos.toLocalPos();
        if (auto it = m_loadedChunks.find(chunkPos); it != m_loadedChunks.end()) {
            return it->second.getBlock(localPos.x, localPos.y, localPos.z);
        }
        return Block { AIR_BLOCK_ID };
    }

    void World::setBlock(WorldPos pos, const Block &block) {
        ChunkPos chunkPos = pos.toChunkPos();
        auto localPos = pos.toLocalPos();

        Chunk& chunk = getChunk(chunkPos);
        chunk.setBlock(localPos.x, localPos.y, localPos.z, block);
    }

    const std::unordered_map<ChunkPos, Chunk, ChunkPosHash>& World::getLoadedChunks() const {
        return m_loadedChunks;
    }

    std::unordered_map<ChunkPos, Chunk, ChunkPosHash> &World::getLoadedChunks() {
        return m_loadedChunks;
    }

    void World::loadChunksAroundPosition(WorldPos pos, int radius) {
        ChunkPos chunk = pos.toChunkPos();

        for (int x = chunk.x - radius; x <= chunk.x + radius; x++) {
            for (int z = chunk.z - radius; z <= chunk.z + radius; z++) {
                getChunk({x, z}); // This will create if not exists
            }
        }
    }

    int World::getSurfaceHeight(int worldX, int worldZ) {
        WorldPos pos {worldX, 255, worldZ};
        ChunkPos chunkPos = pos.toChunkPos();
        auto localPos = pos.toLocalPos();

        Chunk& chunk = getChunk(chunkPos);
        return chunk.getTopBlockY(localPos.x, localPos.z);
    }


}