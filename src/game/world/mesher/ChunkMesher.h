#pragma once

#include "../CubeFace.h"
#include "../BlockRegistry.h"
#include "../BiomeRegistry.h"
#include "LayeredMeshData.h"
#include "BlockVertex.h"
#include "game/world/Chunk.h"

namespace Mineclone {
    class ChunkMesher {
    public:
        ChunkMesher() = delete;

        static LayeredMeshData<BlockVertex> buildMesh(
            const TextureArray& textureArray,
            const BlockRegistry& blockRegistry,
            const BiomeRegistry& biomeRegistry,
            const Chunk& chunk
        );

    private:

        static bool shouldRenderFace(uint8_t x, uint8_t y, uint8_t z, CubeFace face, const Chunk& chunk);

        static void generateBlockFacesInto(
            uint8_t x, uint8_t y, uint8_t z,
            const BlockRegistry& blockRegistry,
            const BiomeRegistry& biomeRegistry,
            const Chunk& chunk,
            BlockId blockId,
            LayeredMeshData<BlockVertex>& layeredMesh
        );

    };

} // namespace Mineclone