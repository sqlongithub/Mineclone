#include "ChunkMesher.h"
#include "BlockVertex.h"
#include "../CubeFace.h"

namespace Mineclone {

    static constexpr float BLOCK_SCALE = 1.0f / 16.0f;
    static constexpr uint32_t FACE_INDICES[6] = {0, 1, 2, 0, 2, 3};

    LayeredMeshData<BlockVertex> ChunkMesher::buildMesh(const TextureArray& textureArray,
                                                        const BlockRegistry& blockRegistry,
                                                        const BiomeRegistry& biomeRegistry,
                                                        const Chunk& chunk) {
        int minSection = chunk.getMinNonEmptySection();
        int maxSection = chunk.getMaxNonEmptySection();
        if (minSection == -1) return {};
        LayeredMeshData<BlockVertex> layeredMesh;

        for (int section = minSection; section <= maxSection; section++) {
            if (chunk.isSectionEmpty(section)) continue;
            int minY = section * SECTION_HEIGHT;
            int maxY = std::min(minY + SECTION_HEIGHT - 1, CHUNK_HEIGHT - 1);
            for (int y = minY; y <= maxY; y++) {
                for (int z = 0; z < CHUNK_DEPTH; z++) {
                    for (int x = 0; x < CHUNK_WIDTH; x++) {
                        const Block* block = chunk.getBlock(x, y, z);
                        if (!block || block->id == AIR_BLOCK_ID) continue;
                        generateBlockFacesInto(x, y, z, blockRegistry, biomeRegistry, chunk, block->id, layeredMesh);
                    }
                }
            }
        }
        return layeredMesh;
    }

    void ChunkMesher::generateBlockFacesInto(uint8_t x, uint8_t y, uint8_t z,
                                             const BlockRegistry& blockRegistry,
                                             const BiomeRegistry& biomeRegistry,
                                             const Chunk& chunk,
                                             BlockId blockId,
                                             LayeredMeshData<BlockVertex>& layeredMesh) {
        const auto* definition = blockRegistry.getDefinition(blockId);
        if (!definition || definition->model.elements.empty()) return;
        BiomeId biomeId = chunk.getBiome(x, z);

        for (const auto& element : definition->model.elements) {
            for (uint8_t faceIdx = 0; faceIdx < 6; faceIdx++) {
                CubeFace face = static_cast<CubeFace>(faceIdx);
                if (!shouldRenderFace(x, y, z, face, chunk)) continue;
                auto faceIt = element.faces.find(getCubeFaceName(face));
                if (faceIt == element.faces.end()) continue;
                const FaceDefinition& faceDef = faceIt->second;
                Layer layer = faceDef.layer;
                auto& vertices = layeredMesh.getVertices(layer);
                auto& indices  = layeredMesh.getIndices(layer);

                const FaceData& fd = getFaceData(face);
                int textureIndex = blockRegistry.getTextureIndex(blockId, element, face);
                if (textureIndex < 0) textureIndex = 0;
                uint32_t baseIndex = static_cast<uint32_t>(vertices.size());

                const float u0 = faceDef.uv[0] * BLOCK_SCALE;
                const float v0 = faceDef.uv[1] * BLOCK_SCALE;
                const float u1 = faceDef.uv[2] * BLOCK_SCALE;
                const float v1 = faceDef.uv[3] * BLOCK_SCALE;

                for (int i = 0; i < 4; i++) {
                    glm::vec3 local = glm::mix(element.from, element.to, fd.corners[i]);
                    glm::vec2 uv;
                    switch(face) {
                        case CubeFace::NORTH: case CubeFace::SOUTH:
                            uv.x = (local.x == element.from.x) ? u0 : u1;
                            uv.y = (local.y == element.from.y) ? v0 : v1;
                            break;
                        case CubeFace::EAST: case CubeFace::WEST:
                            uv.x = (local.z == element.from.z) ? u0 : u1;
                            uv.y = (local.y == element.from.y) ? v0 : v1;
                            break;
                        case CubeFace::UP: case CubeFace::DOWN:
                            uv.x = (local.x == element.from.x) ? u0 : u1;
                            uv.y = (local.z == element.from.z) ? v0 : v1;
                            break;
                        default: break;
                    }
                    BlockVertex v;
                    v.position = glm::vec3{
                        x + local.x * BLOCK_SCALE,
                        y + local.y * BLOCK_SCALE,
                        z + local.z * BLOCK_SCALE
                    };
                    v.uv = {uv.x, 1.0f - uv.y};
                    v.textureIndex = static_cast<uint8_t>(textureIndex);
                    v.faceIndex = static_cast<uint8_t>(face);
                    v.tintIndex = (faceDef.tintIndex >= 0) ? static_cast<uint8_t>(faceDef.tintIndex) : NO_TINT_INDEX;
                    vertices.push_back(v);
                }
                for (uint32_t idx : FACE_INDICES) indices.push_back(baseIndex + idx);
            }
        }
    }

    bool ChunkMesher::shouldRenderFace(uint8_t x, uint8_t y, uint8_t z, CubeFace face, const Chunk& chunk) {
        int nx = x, ny = y, nz = z;
        switch (face) {
            case CubeFace::EAST:  nx = x + 1; break;
            case CubeFace::WEST:  nx = x - 1; break;
            case CubeFace::UP:    ny = y + 1; break;
            case CubeFace::DOWN:  ny = y - 1; break;
            case CubeFace::NORTH: nz = z + 1; break;
            case CubeFace::SOUTH: nz = z - 1; break;
            default: break;
        }
        if (nx < 0 || nx >= CHUNK_WIDTH || ny < 0 || ny >= CHUNK_HEIGHT || nz < 0 || nz >= CHUNK_DEPTH) return true;
        const auto neighbor = chunk.getBlock(nx, ny, nz);
        return !neighbor || neighbor->id == AIR_BLOCK_ID;
    }
}
