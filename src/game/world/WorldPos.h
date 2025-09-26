#include "glm/vec3.hpp"
#include "ChunkPos.h"

#pragma once

namespace Mineclone {

    struct WorldPos {
        int x, y, z;

        explicit WorldPos(int x, int y, int z)
                : x(x), y(y), z(z) {}

        WorldPos(glm::vec3 pos)
                : x(pos.x), y(pos.y), z(pos.z) {}

        WorldPos(glm::ivec3 pos)
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


}