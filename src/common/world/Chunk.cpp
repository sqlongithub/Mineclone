#include "Chunk.h"
#include "ChunkConstants.h"
#include "BlockRegistry.h"

namespace Mineclone {
    Chunk::Chunk() {

    }

    Block* Chunk::getBlock(int x, int y, int z) {
        if (x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_DEPTH) {
            return nullptr;
        }
        return &m_blocks[getIndex(x, y, z)];
    }

    const Block* Chunk::getBlock(int x, int y, int z) const {
        if (x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_DEPTH) {
            return nullptr;
        }
        return &m_blocks[getIndex(x, y, z)];
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

    void Chunk::setBiome(int x, int z, BiomeId id) {
        m_biomes[getIndex(x, z)] = id;
    }

    BiomeId Chunk::getBiome(int x, int z) const {
        return m_biomes[getIndex(x, z)];
    }

    bool Chunk::isSectionEmpty(int sectionIndex) const {
        if (m_sectionsDirty) {
            recalculateSections();
        }
        return !m_nonEmptySections[sectionIndex];
    }

    int Chunk::getMinNonEmptySection() const  {
        if (m_sectionsDirty) recalculateSections();
        for (int i = 0; i < SECTIONS_PER_CHUNK; i++) {
            if (m_nonEmptySections[i]) return i;
        }
        return -1;
    }

    int Chunk::getMaxNonEmptySection() const  {
        if (m_sectionsDirty) recalculateSections();
        for (int i = SECTIONS_PER_CHUNK - 1; i >= 0; i--) {
            if (m_nonEmptySections[i]) return i;
        }
        return -1;
    }

    void Chunk::recalculateSections() const {
        m_nonEmptySections.reset();

        for (int section = 0; section < SECTIONS_PER_CHUNK; section++) {
            int minY = section * SECTION_HEIGHT;
            int maxY = std::min(minY + SECTION_HEIGHT - 1, CHUNK_HEIGHT - 1);

            bool hasBlocks = false;
            for (int y = minY; y <= maxY && !hasBlocks; y++) {
                for (int z = 0; z < CHUNK_DEPTH && !hasBlocks; z++) {
                    for (int x = 0; x < CHUNK_WIDTH && !hasBlocks; x++) {
                        const Block* block = getBlock(x, y, z);
                        if (block && block->id != AIR_BLOCK_ID) {
                            hasBlocks = true;
                        }
                    }
                }
            }

            if (hasBlocks) {
                m_nonEmptySections[section] = true;
            }
        }

        m_sectionsDirty = false;
    }

    int Chunk::getTopBlockY(int x, int z) const {
        for (int y = CHUNK_HEIGHT - 1; y >= 0; y--) {
            if (auto* block = getBlock(x, y, z); block && block->id != AIR_BLOCK_ID) {
                return y;
            }
        }
        return -1;
    }

    size_t Chunk::getIndex(int x, int y, int z) {
        return x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
    }

    size_t Chunk::getIndex(int x, int z) {
        return x + z * CHUNK_WIDTH;
    }
}
