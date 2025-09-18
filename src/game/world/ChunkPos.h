#pragma once

#include <cstdint>
#include <bitset>
#include "Chunk.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "ChunkConstants.h"

namespace Mineclone {
    struct ChunkPos {
        int x, z;

        bool operator==(const ChunkPos& other) const {
            return x == other.x && z == other.z;
        }

        [[nodiscard]] glm::mat4 getModelMatrix() const {
            float worldX = x * CHUNK_WIDTH;
            float worldY = 0;
            float worldZ = z * CHUNK_DEPTH;

            return glm::translate(glm::mat4(1.0f), glm::vec3(worldX, worldY, worldZ));
        }
    };

    struct ChunkPosHash {
        std::size_t operator()(const ChunkPos& pos) const noexcept {
            std::size_t hx = std::hash<int>{}(pos.x);
            std::size_t hz = std::hash<int>{}(pos.z);

            // Boost-style hash combine
            std::size_t seed = hx;
            seed ^= hz + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
}