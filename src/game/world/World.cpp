#include "World.h"
#include "ChunkPos.h"

namespace Mineclone {
    World::World(BlockRegistry& blockRegistry, BiomeRegistry& biomeRegistry, int seed)
        : m_blockRegistry(blockRegistry), m_biomeRegistry(biomeRegistry), m_terrainGenerator(seed) {

    }

    Chunk& World::getChunk(ChunkPos pos) {
        auto it = m_loadedChunks.find(pos);
        if (it != m_loadedChunks.end()) {
            return it->second;
        }

        auto& chunk = m_loadedChunks.emplace(pos, Chunk()).first->second;
        m_terrainGenerator.generate(pos, chunk, m_blockRegistry, m_biomeRegistry);
        return chunk;
    }

    const Chunk* World::getChunk(ChunkPos pos) const {
        auto it = m_loadedChunks.find(pos);
        if (it != m_loadedChunks.end()) {
            return &it->second;
        }
        return nullptr;

    }

    Block* World::getBlock(WorldPos pos) {
        ChunkPos chunkPos = pos.toChunkPos();
        auto localPos = pos.toLocalPos();
        if (auto it = m_loadedChunks.find(chunkPos); it != m_loadedChunks.end()) {
            return it->second.getBlock(localPos.x, localPos.y, localPos.z);
        }
        return nullptr;
    }

    const Block* World::getBlock(WorldPos pos) const {
        const ChunkPos& chunkPos = pos.toChunkPos();
        auto localPos = pos.toLocalPos();
        if (auto it = m_loadedChunks.find(chunkPos); it != m_loadedChunks.end()) {
            return it->second.getBlock(localPos.x, localPos.y, localPos.z);
        }
        return nullptr;
    }

    BiomeId World::getBiome(WorldPos pos) const {
        const auto chunk = getChunk(pos.toChunkPos());
        if(chunk) {
            return chunk->getBiome(pos.toLocalPos().x, pos.toLocalPos().z);
        }
        return m_terrainGenerator.getBiomeAt(pos, m_biomeRegistry);
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
     //   getChunk(chunk);
     //   return;
        for (int x = chunk.x - radius; x <= chunk.x + radius; x++) {
            for (int z = chunk.z - radius; z <= chunk.z + radius; z++) {
                getChunk({x, z}); // create if not exists
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

    std::optional<RaycastHit> World::raycast(glm::vec3 origin, glm::vec3 dir) const {
        glm::ivec3 voxel = glm::floor(origin);
        glm::ivec3 step = glm::sign(dir);
        glm::vec3 deltaDist = glm::abs(1.0f / dir);

        glm::vec3 sideDist;
        for (int i = 0; i < 3; i++) {
            sideDist[i] = (step[i] > 0) ?
                          (voxel[i] + 1 - origin[i]) * deltaDist[i] :
                          (origin[i] - voxel[i]) * deltaDist[i];
        }

        float distance = 0;
        int lastAxis = -1;
        bool firstStep = true;

        while (distance < 100.0f) {
            auto block = getBlock(voxel);
            if (block && block->id != AIR_BLOCK_ID) {
                glm::vec3 hitPoint = origin + dir * distance;
                CubeFace face = CubeFace::UNKNOWN;

                if (firstStep) {
                    if (abs(dir.x) > abs(dir.y) && abs(dir.x) > abs(dir.z))
                        face = (dir.x > 0) ? CubeFace::EAST : CubeFace::WEST;
                    else if (abs(dir.y) > abs(dir.z))
                        face = (dir.y > 0) ? CubeFace::UP : CubeFace::DOWN;
                    else
                        face = (dir.z > 0) ? CubeFace::NORTH : CubeFace::SOUTH;
                } else if (lastAxis != -1) {
                    if (lastAxis == 0)
                        face = (step[0] > 0) ? CubeFace::WEST : CubeFace::EAST;
                    else if (lastAxis == 1)
                        face = (step[1] > 0) ? CubeFace::DOWN : CubeFace::UP;
                    else if (lastAxis == 2)
                        face = (step[2] > 0) ? CubeFace::SOUTH : CubeFace::NORTH;
                }

                return RaycastHit{
                    voxel,
                    hitPoint,
                    *block,
                    face
                };
            }

            firstStep = false;

            int axis = (sideDist.x < sideDist.y) ?
                       ((sideDist.x < sideDist.z) ? 0 : 2) :
                       ((sideDist.y < sideDist.z) ? 1 : 2);

            lastAxis = axis;
            distance = sideDist[axis];
            sideDist[axis] += deltaDist[axis];
            voxel[axis] += step[axis];
        }

        return std::nullopt;
    }




}