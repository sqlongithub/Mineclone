
#include "HorizontalSlice.h"

namespace Mineclone {

    Block HorizontalSlice::getBlock(int x, int y, int z) const {
        if (y < minY || y > maxY) return Block { AIR_BLOCK_ID };
        return chunk->getBlock(x, y, z);
    }

    bool HorizontalSlice::isEmpty() const {
        for (int y = minY; y <= maxY; y++) {
            for (int x = 0; x < CHUNK_WIDTH; x++) {
                for (int z = 0; z < CHUNK_DEPTH; z++) {
                    if (chunk->getBlock(x, y, z).id != AIR_BLOCK_ID) {
                        return false;
                    }
                }
            }
        }
        return true;
    }



}