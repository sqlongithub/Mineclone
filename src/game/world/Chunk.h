#pragma once

#include <cstdint>
#include <bitset>
#include <memory>
#include <vector>
#include "Block.h"
#include "HorizontalSlice.h"
#include "ChunkPos.h"
#include "BlockRegistry.h"


namespace Mineclone {
    struct HorizontalSlice;

    class Chunk {
    public:
        Chunk();

        [[nodiscard]] Block getBlock(int x, int y, int z) const;
        void setBlock(int x, int y, int z, const Block& block);
        void setBlock(int x, int y, int z, BlockId id);
        void setBlock(int x, int y, int z, std::string type);

        [[nodiscard]] bool isMeshDirty() const { return m_meshDirty; }
        void markMeshClean() { m_meshDirty = false; }
        void markMeshDirty() { m_meshDirty = true; }

        [[nodiscard]] HorizontalSlice getSlice(int minY, int maxY) const;
        [[nodiscard]] std::vector<std::pair<int, int>> getNonEmptySliceRanges(int sliceHeight = 16) const;
        [[nodiscard]] std::vector<HorizontalSlice> getNonEmptySlices(int sliceHeight = 16) const;

        [[nodiscard]] int getTopBlockY(int x, int z) const;

    private:
        [[nodiscard]] static size_t getIndex(int x, int y, int z) ;

        std::unique_ptr<Block[]> m_blocks;
        bool m_meshDirty;


    };
}