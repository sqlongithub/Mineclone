#include "ChunkMesher.h"

#include <iostream>
#include "BlockVertex.h"

namespace Mineclone {

    static constexpr float BLOCK_SCALE = 1.0f / 16.0f;

    LayeredMeshData<BlockVertex> ChunkMesher::buildMesh(const TextureArray& textureArray,
                                                        const BlockRegistry& blockRegistry,
                                                        const BiomeRegistry& biomeRegistry,
                                                        const Chunk& chunk) {
        int minSection = chunk.getMinNonEmptySection();
        int maxSection = chunk.getMaxNonEmptySection();

        if (minSection == -1) {
            return {}; // Empty chunk
        }

        LayeredMeshData<BlockVertex> layeredMesh;

        for (int section = minSection; section <= maxSection; section++) {
            if (chunk.isSectionEmpty(section)) {
                continue; // Skip empty sections entirely
            }

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



        if (!definition || definition->model.elements.empty()) {
            return;
        }

        BiomeId biomeId = chunk.getBiome(x, z);

      //  std::cout << "Processing block at (" << x << "," << y << "," << z << ") with "
//          << definition->model.elements.size() << " elements" << std::endl;

        for (size_t i = 0; i < definition->model.elements.size(); ++i) {
            const auto& element = definition->model.elements[i];
           // std::cout << "  Element " << i << " has " << element.faces.size() << " faces" << std::endl;
            for (uint8_t faceIdx = 0; faceIdx < 6; faceIdx++) {
                const auto face = static_cast<CubeFace>(faceIdx);

                if (!shouldRenderFace(x, y, z, face, chunk))
                    continue;

                std::string faceName = getCubeFaceName(face);



                auto faceIt = element.faces.find(faceName);
                if (faceIt == element.faces.end())
                    continue;

                auto faceDefinition = faceIt->second;

                Layer layer = faceDefinition.layer;

                generateElementFaceVerticesInto(x, y, z, blockRegistry, face, blockId,
                                                element, faceIt->second, biomeId, layeredMesh, layer);
            }
        }
    }

    bool ChunkMesher::shouldRenderFace(uint8_t x, uint8_t y, uint8_t z, CubeFace face, const Chunk& chunk) {
        // Calculate neighbor position based on face direction
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

        // TODO: Check adjacent chunks for proper culling
        if (nx < 0 || nx >= CHUNK_WIDTH || ny < 0 || ny >= CHUNK_HEIGHT || nz < 0 || nz >= CHUNK_DEPTH) {
            return true;
        }

        const auto neighbor = chunk.getBlock(nx, ny, nz);
        if (!neighbor) {
            return true;
        }

        // TODO: Check block opacity/transparency instead of assuming all non-air blocks are opaque
        return neighbor->id == AIR_BLOCK_ID;
    }

    void ChunkMesher::generateElementFaceVerticesInto(uint8_t x, uint8_t y, uint8_t z,
                                                  const BlockRegistry& blockRegistry,
                                                  CubeFace face,
                                                  BlockId blockId,
                                                  const BlockElement& element,
                                                  const FaceDefinition& faceDefinition,
                                                  BiomeId biomeId,
                                                  LayeredMeshData<BlockVertex>& layeredMesh,
                                                  Layer layer) {

        auto& vertices = layeredMesh.getVertices(layer);
        auto& indices  = layeredMesh.getIndices(layer);

        std::array<glm::vec3, 4> corners = calculateElementFaceCorners(element, face);

        const float u0 = faceDefinition.uv[0] * BLOCK_SCALE;
        const float v0 = faceDefinition.uv[1] * BLOCK_SCALE;
        const float u1 = faceDefinition.uv[2] * BLOCK_SCALE;
        const float v1 = faceDefinition.uv[3] * BLOCK_SCALE;

        auto pickUVForCorner = [&](const glm::vec3& corner) -> glm::vec2 {
            float u = u0, v = v0;
            switch(face) {
                case CubeFace::NORTH: case CubeFace::SOUTH:
                    u = (corner.x == element.from.x) ? u0 : u1;
                    v = (corner.y == element.from.y) ? v0 : v1;
                    break;
                case CubeFace::EAST: case CubeFace::WEST:
                    u = (corner.z == element.from.z) ? u0 : u1;
                    v = (corner.y == element.from.y) ? v0 : v1;
                    break;
                case CubeFace::UP: case CubeFace::DOWN:
                    u = (corner.x == element.from.x) ? u0 : u1;
                    v = (corner.z == element.from.z) ? v0 : v1;
                    break;
            }
            return {u, 1.0f - v};
        };

        int textureIndex = blockRegistry.getTextureIndex(blockId, element, face);
        if (textureIndex < 0) textureIndex = 0;

        uint32_t baseIndex = static_cast<uint32_t>(vertices.size());

        for (int i = 0; i < 4; ++i) {
            BlockVertex vertex;
            vertex.position = glm::vec3{
                x + corners[i].x * BLOCK_SCALE,
                y + corners[i].y * BLOCK_SCALE,
                z + corners[i].z * BLOCK_SCALE
            };
            vertex.uv = pickUVForCorner(corners[i]);
            vertex.textureIndex = static_cast<uint8_t>(textureIndex);
            vertex.faceIndex = static_cast<uint8_t>(face);
            vertex.tintIndex = (faceDefinition.tintIndex >= 0) ? static_cast<uint8_t>(faceDefinition.tintIndex)
                                                               : NO_TINT_INDEX;
            vertices.push_back(vertex);
        }

        indices.insert(indices.end(), {
            baseIndex + 0, baseIndex + 1, baseIndex + 2,
            baseIndex + 0, baseIndex + 2, baseIndex + 3
        });

#ifdef DEBUG
      //  std::cout << "Generating face " << getCubeFaceName(face) << std::endl;
        for (size_t i = 0; i < std::min(static_cast<int>(vertices.size()), 1); i++) {
            const auto& vertex = vertices[i];
     //       std::cout << "  Vertex: pos(" << vertex.position.x << ","
                  //    << vertex.position.y << "," << vertex.position.z << ")" << std::endl;
        }
        for (size_t i = 0; i < std::min(static_cast<int>(indices.size()), 3); i += 3) {
      //      std::cout << "  Triangle: " << indices[i] << ","
                //      << indices[i+1] << "," << indices[i+2] << std::endl;
        }

#endif
    }


    std::array<glm::vec3, 4> ChunkMesher::calculateElementFaceCorners(const BlockElement& element, CubeFace face) {
        std::array<glm::vec3, 4> corners;
        const glm::vec3& from = element.from;
        const glm::vec3& to = element.to;

        switch (face) {
            case CubeFace::NORTH:
                corners = { glm::vec3{from.x, from.y, to.z}, glm::vec3{to.x, from.y, to.z},
                            glm::vec3{to.x, to.y, to.z}, glm::vec3{from.x, to.y, to.z} };
                break;
            case CubeFace::SOUTH:
                corners = { glm::vec3{to.x, from.y, from.z}, glm::vec3{from.x, from.y, from.z},
                            glm::vec3{from.x, to.y, from.z}, glm::vec3{to.x, to.y, from.z} };
                break;
            case CubeFace::EAST:
                corners = { glm::vec3{to.x, from.y, to.z}, glm::vec3{to.x, from.y, from.z},
                            glm::vec3{to.x, to.y, from.z}, glm::vec3{to.x, to.y, to.z} };
                break;
            case CubeFace::WEST:
                corners = { glm::vec3{from.x, from.y, from.z}, glm::vec3{from.x, from.y, to.z},
                            glm::vec3{from.x, to.y, to.z}, glm::vec3{from.x, to.y, from.z} };
                break;
            case CubeFace::UP:
                corners = { glm::vec3{from.x, to.y, from.z}, glm::vec3{to.x, to.y, from.z},
                            glm::vec3{to.x, to.y, to.z}, glm::vec3{from.x, to.y, to.z} };
                break;
            case CubeFace::DOWN:
                corners = { glm::vec3{from.x, from.y, to.z}, glm::vec3{to.x, from.y, to.z},
                            glm::vec3{to.x, from.y, from.z}, glm::vec3{from.x, from.y, from.z} };
                break;
        }

        return corners;
    }

}
