#pragma once

#include <cstdint>
#include <bitset>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ChunkConstants.h"

namespace Mineclone {
    struct WorldPos;

    struct ChunkPos {
        int x, z;

        bool operator==(const ChunkPos& other) const;
        [[nodiscard]] glm::mat4 getModelMatrix() const;
        [[nodiscard]] double distanceTo(const ChunkPos& other) const;

        static ChunkPos fromWorldPos(const WorldPos& pos);
    };

    struct ChunkPosHash {
        std::size_t operator()(const ChunkPos& pos) const noexcept;
    };
}
