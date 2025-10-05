#include "ChunkPos.h"
#include "WorldPos.h"

namespace Mineclone {

    bool ChunkPos::operator==(const ChunkPos& other) const {
        return x == other.x && z == other.z;
    }

    glm::mat4 ChunkPos::getModelMatrix() const {
        float worldX = x * CHUNK_WIDTH;
        float worldY = 0;
        float worldZ = z * CHUNK_DEPTH;

        return glm::translate(glm::mat4(1.0f), glm::vec3(worldX, worldY, worldZ));
    }

    double ChunkPos::distanceTo(const ChunkPos& other) const {
        const double dx = x - other.x;
        const double dz = z - other.z;
        return glm::sqrt(dx * dx + dz * dz);
    }

    ChunkPos ChunkPos::fromWorldPos(const WorldPos& pos) {
        return ChunkPos{pos.x / CHUNK_WIDTH, pos.z / CHUNK_DEPTH};
    }

    std::size_t ChunkPosHash::operator()(const ChunkPos& pos) const noexcept {
        std::size_t hx = std::hash<int>{}(pos.x);
        std::size_t hz = std::hash<int>{}(pos.z);

        std::size_t seed = hx;
        seed ^= hz + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
}
