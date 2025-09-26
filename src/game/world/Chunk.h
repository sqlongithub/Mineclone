#pragma once

#include <bitset>
#include "Block.h"
#include "ChunkPos.h"
#include "BlockRegistry.h"
#include "Biome.h"


namespace Mineclone {
    struct HorizontalSlice;

    class Chunk {
    public:
        Chunk();

        [[nodiscard]] Block* getBlock(int x, int y, int z);
        [[nodiscard]] const Block* getBlock(int x, int y, int z) const;
        void setBlock(int x, int y, int z, const Block& block);
        void setBlock(int x, int y, int z, BlockId id);
        void setBlock(int x, int y, int z, std::string type);

        void setBiome(int x, int z, BiomeId id);
        BiomeId getBiome(int x, int z) const;

        [[nodiscard]] bool isMeshDirty() const { return m_meshDirty; }
        void markMeshClean() { m_meshDirty = false; }
        void markMeshDirty() { m_meshDirty = true; }

        bool isSectionEmpty(int sectionIndex) const;

        // these functions return the range where all non-empty sections lie in (can contain empty sections between those)
        int getMinNonEmptySection() const;
        int getMaxNonEmptySection() const;

        [[nodiscard]] int getTopBlockY(int x, int z) const;

    private:
        // a section is just a chunk_width x chunk_depth x section_height section, that doesnt store any data, which functions as an index
        // so we can test whether or not a 16 high part of the chunk has any blocks, so we can skip meshing and rendering
        mutable bool m_sectionsDirty = true;
        mutable std::bitset<SECTIONS_PER_CHUNK> m_nonEmptySections;

        void recalculateSections() const;

        [[nodiscard]] static size_t getIndex(int x, int y, int z) ;
        [[nodiscard]] static size_t getIndex(int x, int z) ;

        Block m_blocks[CHUNK_VOLUME];
        BiomeId m_biomes[CHUNK_WIDTH * CHUNK_DEPTH];


        bool m_meshDirty;


    };
}