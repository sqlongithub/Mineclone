#pragma once

#include "Chunk.h"

namespace Mineclone {
    class Chunk;
    struct HorizontalSlice {
        const Chunk* chunk;
        int minY;
        int maxY;

        HorizontalSlice() = default;
        HorizontalSlice(const Chunk* c, int min, int max)
                : chunk(c), minY(min), maxY(max) {}

        [[nodiscard]] Block getBlock(int x, int y, int z) const;
        [[nodiscard]] bool isEmpty() const;
        [[nodiscard]] int getHeight() const { return maxY - minY; };
    };

}
