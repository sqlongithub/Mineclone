#pragma once

#include "mesh/Mesh.h"
#include "HorizontalSlice.h"
#include "CubeFace.h"
#include "TextureArray.h"
#include "BlockRegistry.h"

namespace Mineclone {
    class ChunkMesher {
    public:
        ChunkMesher() = delete;

        static Mesh buildMesh(const TextureArray& textureArray, const BlockRegistry& blockRegistry, const Chunk& slice);

    private:
        static std::array<Vertex, 4> generateFaceVertices(uint8_t x, uint8_t y, uint8_t z, const BlockRegistry& textureArray, CubeFace face, BlockId id) ;
    };

} // Mineclone
