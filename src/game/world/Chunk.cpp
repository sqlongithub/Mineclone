#include "Chunk.h"
#include "ChunkConstants.h"
#include "BlockRegistry.h"

namespace Mineclone {
    Chunk::Chunk()
        : m_blocks(std::make_unique<Block[]>(CHUNK_VOLUME)) {

    }

    Block Chunk::getBlock(int x, int y, int z) const {
        if (x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_DEPTH) {
            return Block { AIR_BLOCK_ID };
        }
        return m_blocks[getIndex(x, y, z)];
    }

    void Chunk::setBlock(int x, int y, int z, const Block &block) {
        if (x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_DEPTH) {
            return;
        }
        m_blocks[getIndex(x, y, z)] = block;
        m_meshDirty = true;
    }

    void Chunk::setBlock(int x, int y, int z, BlockId id) {
        setBlock(x, y, z, Block(id));
    }

    HorizontalSlice Chunk::getSlice(int minY, int maxY) const {
        return HorizontalSlice(this,
                               std::max(0, minY),
                               std::min(CHUNK_HEIGHT - 1, maxY));
    }

    // returns all non-empty slices minY, maxY
    std::vector<std::pair<int, int>> Chunk::getNonEmptySliceRanges(int sliceHeight) const {
        std::vector<std::pair<int, int>> ranges;

        for (int y = 0; y < CHUNK_HEIGHT; y += sliceHeight) {
            int maxY = std::min(y + sliceHeight - 1, CHUNK_HEIGHT - 1);
            HorizontalSlice slice = getSlice(y, maxY);

            if (!slice.isEmpty()) {
                ranges.emplace_back(y, maxY);
            }
        }

        return ranges;
    }

    std::vector<HorizontalSlice> Chunk::getNonEmptySlices(int sliceHeight) const {
        std::vector<HorizontalSlice> slices;
        slices.reserve((CHUNK_HEIGHT / sliceHeight) / 2);
        for (int y = 0; y < CHUNK_HEIGHT; y += sliceHeight) {
            int maxY = std::min(y + sliceHeight - 1, CHUNK_HEIGHT - 1);
            HorizontalSlice slice = getSlice(y, maxY);

            if (!slice.isEmpty()) {
                slices.push_back(slice);
            }
        }

        return slices;
    }

    int Chunk::getTopBlockY(int x, int z) const {
        for (int y = CHUNK_HEIGHT - 1; y >= 0; y--) {
            if (getBlock(x, y, z).id != AIR_BLOCK_ID) {
                return y;
            }
        }
        return -1; // all air
    }

    size_t Chunk::getIndex(int x, int y, int z) {
        return x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
    }
}
