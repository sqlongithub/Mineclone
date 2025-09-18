
#include <iostream>
#include "ChunkMesher.h"
#include "TextureArray.h"
#include "BlockRegistry.h"

namespace Mineclone {

    Mesh ChunkMesher::buildMesh(const TextureArray& textureArray, const BlockRegistry& blockRegistry, const Chunk& chunk) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        vertices.reserve(60000);
        indices.reserve(90000);
        uint32_t indexOffset = 0;

        for (auto slice : chunk.getNonEmptySlices()) {
            std::cout << "Building slice from " << slice.minY << " to " << slice.maxY << "\n";
            for (int y = slice.minY; y <= slice.maxY; y++) {
                for (uint8_t z = 0; z < CHUNK_DEPTH; z++) {
                    for (uint8_t x = 0; x < CHUNK_WIDTH; x++) {
                        const Block &block = slice.getBlock(x, y, z);
                        if (block.id == AIR_BLOCK_ID)
                            continue;

                        for (uint8_t i = 0; i < 6; i++) {
                            int nx = x, ny = y, nz = z;
                            auto face = static_cast<CubeFace>(i);

                            // Determine neighbor position for this face
                            switch (face) {
                                case CubeFace::EAST:
                                    nx = x + 1;
                                    break; // +X
                                case CubeFace::WEST:
                                    nx = x - 1;
                                    break; // -X
                                case CubeFace::UP:
                                    ny = y + 1;
                                    break; // +Y
                                case CubeFace::DOWN:
                                    ny = y - 1;
                                    break; // -Y
                                case CubeFace::NORTH:
                                    nz = z + 1;
                                    break; // +Z
                                case CubeFace::SOUTH:
                                    nz = z - 1;
                                    break; // -Z
                                default:
                                    break;
                            }
                            if(y >= 61) {
                             //   std::cout << "hello";
                            }
                            if(nx < 0 || nx >= CHUNK_WIDTH || ny < 0 || ny >= CHUNK_HEIGHT || nz < 0 || nz >= CHUNK_DEPTH) {

                            } else {
                                // Neighbor is inside chunk - check if it's solid
                                const Block& neighbor = chunk.getBlock(nx, ny, nz);
                                if (neighbor.id != AIR_BLOCK_ID)
                                    continue; // face is occluded by solid neighbor
                            }

                            // Add vertices for this face
                            std::array<Vertex, 4> faceVertices = generateFaceVertices(x, y, z, blockRegistry, face, block.id);
                            vertices.insert(vertices.end(), faceVertices.begin(), faceVertices.end());

                            // Add indices (2 triangles)
                            indices.push_back(indexOffset + 0);
                            indices.push_back(indexOffset + 2);
                            indices.push_back(indexOffset + 1);
                            indices.push_back(indexOffset + 0);
                            indices.push_back(indexOffset + 3);
                            indices.push_back(indexOffset + 2);

                            indexOffset += 4;
                        }

                    }
                }
            }
        }

        return { vertices, indices };
    }

    std::array<Vertex, 4> ChunkMesher::generateFaceVertices(uint8_t x, uint8_t y, uint8_t z, const BlockRegistry& blockRegistry, CubeFace face, BlockId id) {
        const FaceData& faceData = FACE_DATA[static_cast<uint8_t>(face)];

        std::array<Vertex, 4> vertices;

        glm::vec2 uv[4] = {
                {0.0f, 1.0f}, // top-left
                {1.0f, 1.0f}, // top-right
                {1.0f, 0.0f}, // bottom-right
                {0.0f, 0.0f}  // bottom-left
        };



        for (int i = 0; i < 4; ++i) {
            vertices[i].position = glm::vec3{
                    x + faceData.corners[i].x,
                    y + faceData.corners[i].y,
                    z + faceData.corners[i].z
            };
            vertices[i].normal = faceData.normal;
            //vertices[i].tangent = faceData.tangent;
            vertices[i].uv = uv[i];
            vertices[i].color = glm::vec3(0.6431, 0.8392, 0.549);
            vertices[i].textureIndex = blockRegistry.getTextureIndex(id, face);
        }

        return vertices;
    }

} // Mineclone