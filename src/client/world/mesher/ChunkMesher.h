#pragma once
#include <cstdint>
#include <vector>
#include <array>

#include "client/rendering/materials/TextureArray.h"
#include "client/rendering/mesh/BlockVertex.h"
#include "client/rendering/mesh/LayeredMeshData.h"
#include "common/world/BiomeRegistry.h"
#include "common/world/BlockRegistry.h"
#include "common/world/Chunk.h"

namespace Mineclone {

    class ChunkMesher {
    public:
        static LayeredMeshData<BlockVertex> buildMesh(const TextureArray& textureArray,
                                                      const BlockRegistry& blockRegistry,
                                                      const BiomeRegistry& biomeRegistry,
                                                      const Chunk& chunk,
                                                      const std::array<const Chunk*, 4>& neighbors);

    private:
        struct MeshContext {
            const Chunk& chunk;
            const std::array<const Chunk*, 4>& neighbors;
            const BlockRegistry& blockRegistry;
        };

        static void generateBlockFacesInto(uint8_t x, uint8_t y, uint8_t z,
                                          const BlockRegistry& blockRegistry,
                                          const BiomeRegistry& biomeRegistry,
                                          const MeshContext& context,
                                          BlockId blockId,
                                          LayeredMeshData<BlockVertex>& layeredMesh);

        static void addFace(uint8_t x, uint8_t y, uint8_t z, CubeFace face,
                           const BlockElement& element, const FaceDefinition& faceDef,
                           const MeshContext& context,
                           int textureIndex, std::vector<BlockVertex>& vertices,
                           std::vector<uint32_t>& indices);

        static float computeU(CubeFace face, const glm::vec3& local, const BlockElement& element, float u0, float u1);
        static float computeV(CubeFace face, const glm::vec3& local, const BlockElement& element, float v0, float v1);

        static bool shouldRenderFace(uint8_t x, uint8_t y, uint8_t z, CubeFace face, const MeshContext& context);

        static float computeAO(const MeshContext& context, int x, int y, int z, CubeFace face, int corner);

        static const Block* getBlockAt(const MeshContext& context, int x, int y, int z);
    };

}