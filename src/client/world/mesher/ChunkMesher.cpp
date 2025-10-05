#include "ChunkMesher.h"
#include "common/world/BiomeRegistry.h"

namespace Mineclone {

    static constexpr float BLOCK_SCALE = 1.0f / 16.0f;
    static constexpr uint32_t FACE_INDICES[6] = {0, 1, 2, 0, 2, 3};
    static constexpr float AO_VALUES[4] = {1.0f, 0.75f, 0.5f, 0.25f};

    LayeredMeshData<BlockVertex> ChunkMesher::buildMesh(const TextureArray& textureArray,
                                                        const BlockRegistry& blockRegistry,
                                                        const BiomeRegistry& biomeRegistry,
                                                        const Chunk& chunk,
                                                        const std::array<const Chunk*, 4>& neighbors) {
        int minSection = chunk.getMinNonEmptySection();
        int maxSection = chunk.getMaxNonEmptySection();
        if (minSection == -1) return {};

        LayeredMeshData<BlockVertex> layeredMesh;
        MeshContext context{chunk, neighbors, blockRegistry};

        int minY = minSection * SECTION_HEIGHT;
        int maxY = std::min((maxSection + 1) * SECTION_HEIGHT - 1, CHUNK_HEIGHT - 1);

        for (int y = minY; y <= maxY; y++) {
            for (int z = 0; z < CHUNK_DEPTH; z++) {
                for (int x = 0; x < CHUNK_WIDTH; x++) {
                    const Block* block = chunk.getBlock(x, y, z);
                    if (!block || block->id == AIR_BLOCK_ID) continue;
                    generateBlockFacesInto(x, y, z, blockRegistry, biomeRegistry, context, block->id, layeredMesh);
                }
            }
        }
        return layeredMesh;
    }

    void ChunkMesher::generateBlockFacesInto(uint8_t x, uint8_t y, uint8_t z,
                                             const BlockRegistry& blockRegistry,
                                             const BiomeRegistry& biomeRegistry,
                                             const MeshContext& context,
                                             BlockId blockId,
                                             LayeredMeshData<BlockVertex>& layeredMesh) {
        const auto* definition = blockRegistry.getDefinition(blockId);
        if (!definition || definition->model.elements.empty()) return;

        for (const auto& element : definition->model.elements) {
            for (uint8_t faceIdx = 0; faceIdx < 6; faceIdx++) {
                CubeFace face = static_cast<CubeFace>(faceIdx);
                if (!shouldRenderFace(x, y, z, face, context)) continue;

                auto faceIt = element.faces.find(getCubeFaceName(face));
                if (faceIt == element.faces.end()) continue;

                const FaceDefinition& faceDef = faceIt->second;
                auto& vertices = layeredMesh.getVertices(faceDef.layer);
                auto& indices = layeredMesh.getIndices(faceDef.layer);

                addFace(x, y, z, face, element, faceDef, context,
                       blockRegistry.getTextureIndex(blockId, element, face), vertices, indices);
            }
        }
    }

    void ChunkMesher::addFace(uint8_t x, uint8_t y, uint8_t z, CubeFace face,
                              const BlockElement& element, const FaceDefinition& faceDef,
                              const MeshContext& context,
                              int textureIndex, std::vector<BlockVertex>& vertices,
                              std::vector<uint32_t>& indices) {
        const FaceData& fd = getFaceData(face);
        uint32_t baseIndex = static_cast<uint32_t>(vertices.size());

        const float u0 = faceDef.uv[0] * BLOCK_SCALE;
        const float v0 = faceDef.uv[1] * BLOCK_SCALE;
        const float u1 = faceDef.uv[2] * BLOCK_SCALE;
        const float v1 = faceDef.uv[3] * BLOCK_SCALE;

        for (int i = 0; i < 4; i++) {
            glm::vec3 local = glm::mix(element.from, element.to, fd.corners[i]);

            BlockVertex v;
            v.position = glm::vec3{x, y, z} + local * BLOCK_SCALE;
            v.uv = {computeU(face, local, element, u0, u1), 1.0f - computeV(face, local, element, v0, v1)};
            v.textureIndex = static_cast<uint8_t>(textureIndex);
            v.faceIndex = static_cast<uint8_t>(face);
            v.tintIndex = (faceDef.tintIndex >= 0) ? static_cast<uint8_t>(faceDef.tintIndex) : NO_TINT_INDEX;
            v.ao = computeAO(context, x, y, z, face, i);

            vertices.push_back(v);
        }

        for (uint32_t idx : FACE_INDICES) {
            indices.push_back(baseIndex + idx);
        }
    }

    float ChunkMesher::computeU(CubeFace face, const glm::vec3& local, const BlockElement& element, float u0, float u1) {
        switch(face) {
            case CubeFace::NORTH: case CubeFace::SOUTH:
            case CubeFace::UP: case CubeFace::DOWN:
                return (local.x == element.from.x) ? u0 : u1;
            case CubeFace::EAST: case CubeFace::WEST:
                return (local.z == element.from.z) ? u0 : u1;
            default: return u0;
        }
    }

    float ChunkMesher::computeV(CubeFace face, const glm::vec3& local, const BlockElement& element, float v0, float v1) {
        switch(face) {
            case CubeFace::NORTH: case CubeFace::SOUTH:
            case CubeFace::EAST: case CubeFace::WEST:
                return (local.y == element.from.y) ? v0 : v1;
            case CubeFace::UP: case CubeFace::DOWN:
                return (local.z == element.from.z) ? v0 : v1;
            default: return v0;
        }
    }

    bool ChunkMesher::shouldRenderFace(uint8_t x, uint8_t y, uint8_t z, CubeFace face, const MeshContext& context) {
        static const glm::ivec3 offsets[6] = {
            {0, 0, 1},
            {0, 0, -1},
            {1, 0, 0},
            {-1, 0, 0},
            {0, 1, 0},
            {0, -1, 0}
        };

        glm::ivec3 pos = glm::ivec3{x, y, z} + offsets[static_cast<int>(face)];
        const Block* block = getBlockAt(context, pos.x, pos.y, pos.z);
        return !block || block->id == AIR_BLOCK_ID;
    }

    const Block* ChunkMesher::getBlockAt(const MeshContext& context, int x, int y, int z) {
        if (y < 0 || y >= CHUNK_HEIGHT) {
            return nullptr;
        }

        if (x >= 0 && x < CHUNK_WIDTH && z >= 0 && z < CHUNK_DEPTH) {
            return context.chunk.getBlock(x, y, z);
        }

        const Chunk* neighborChunk = nullptr;
        int localX = x;
        int localZ = z;

        if (x < 0) {
            neighborChunk = context.neighbors[3];
            localX = x + CHUNK_WIDTH;
        } else if (x >= CHUNK_WIDTH) {
            neighborChunk = context.neighbors[2];
            localX = x - CHUNK_WIDTH;
        } else if (z < 0) {
            neighborChunk = context.neighbors[1];
            localZ = z + CHUNK_DEPTH;
        } else if (z >= CHUNK_DEPTH) {
            neighborChunk = context.neighbors[0];
            localZ = z - CHUNK_DEPTH;
        }

        if (neighborChunk) {
            return neighborChunk->getBlock(localX, y, localZ);
        }

        return nullptr;
    }

    float ChunkMesher::computeAO(const MeshContext& context, int x, int y, int z, CubeFace face, int corner) {
        glm::ivec3 side1{}, side2{}, diag{};
        glm::ivec3 faceOffset{};

        switch(face) {
            case CubeFace::NORTH:
                faceOffset = {0, 0, 1};
                switch(corner){
                    case 0: side1={1,0,0}; side2={0,1,0}; diag={1,1,0}; break;
                    case 1: side1={-1,0,0}; side2={0,1,0}; diag={-1,1,0}; break;
                    case 2: side1={-1,0,0}; side2={0,-1,0}; diag={-1,-1,0}; break;
                    case 3: side1={1,0,0}; side2={0,-1,0}; diag={1,-1,0}; break;
                }
                break;
            case CubeFace::SOUTH:
                faceOffset = {0, 0, -1};
                switch(corner){
                    case 0: side1={-1,0,0}; side2={0,1,0}; diag={-1,1,0}; break;
                    case 1: side1={1,0,0}; side2={0,1,0}; diag={1,1,0}; break;
                    case 2: side1={1,0,0}; side2={0,-1,0}; diag={1,-1,0}; break;
                    case 3: side1={-1,0,0}; side2={0,-1,0}; diag={-1,-1,0}; break;
                }
                break;
            case CubeFace::EAST:
                faceOffset = {1, 0, 0};
                switch(corner){
                    case 0: side1={0,0,-1}; side2={0,1,0}; diag={0,1,-1}; break;
                    case 1: side1={0,0,1}; side2={0,1,0}; diag={0,1,1}; break;
                    case 2: side1={0,0,1}; side2={0,-1,0}; diag={0,-1,1}; break;
                    case 3: side1={0,0,-1}; side2={0,-1,0}; diag={0,-1,-1}; break;
                }
                break;
            case CubeFace::WEST:
                faceOffset = {-1, 0, 0};
                switch(corner){
                    case 0: side1={0,0,1}; side2={0,1,0}; diag={0,1,1}; break;
                    case 1: side1={0,0,-1}; side2={0,1,0}; diag={0,1,-1}; break;
                    case 2: side1={0,0,-1}; side2={0,-1,0}; diag={0,-1,-1}; break;
                    case 3: side1={0,0,1}; side2={0,-1,0}; diag={0,-1,1}; break;
                }
                break;
            case CubeFace::UP:
                faceOffset = {0, 1, 0};
                switch(corner){
                    case 0: side1={-1,0,0}; side2={0,0,1}; diag={-1,0,1}; break;
                    case 1: side1={1,0,0}; side2={0,0,1}; diag={1,0,1}; break;
                    case 2: side1={1,0,0}; side2={0,0,-1}; diag={1,0,-1}; break;
                    case 3: side1={-1,0,0}; side2={0,0,-1}; diag={-1,0,-1}; break;
                }
                break;
            case CubeFace::DOWN:
                faceOffset = {0, -1, 0};
                switch(corner){
                    case 0: side1={-1,0,0}; side2={0,0,-1}; diag={-1,0,-1}; break;
                    case 1: side1={1,0,0}; side2={0,0,-1}; diag={1,0,-1}; break;
                    case 2: side1={1,0,0}; side2={0,0,1}; diag={1,0,1}; break;
                    case 3: side1={-1,0,0}; side2={0,0,1}; diag={-1,0,1}; break;
                }
                break;
            default: break;
        }

        auto isSolid = [&](const glm::ivec3& offset) -> bool {
            glm::ivec3 pos = glm::ivec3{x, y, z} + faceOffset + offset;
            const Block* b = getBlockAt(context, pos.x, pos.y, pos.z);
            if (!b) return false;
            const auto* def = context.blockRegistry.getDefinition(b->id);
            return def && def->metadata.solid;
        };

        bool s1 = isSolid(side1);
        bool s2 = isSolid(side2);
        bool d = isSolid(diag);

        if (s1 && s2) return AO_VALUES[3];
        return AO_VALUES[s1 + s2 + d];
    }
}