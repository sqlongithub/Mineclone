#include "ChunkRenderSystem.h"
#include "../world/mesher/ChunkMesher.h"
#include "../world/mesher/BlockVertex.h"

namespace Mineclone {

    ChunkRenderSystem::ChunkRenderSystem(World& world, AssetManager& assets, BlockRegistry& blockRegistry, BiomeRegistry& biomeRegistry)
        : m_assets(assets), m_blockRegistry(blockRegistry), m_biomeRegistry(biomeRegistry), m_world(world) {
    }


    void ChunkRenderSystem::update(Registry& registry, float dt) {
        auto& loadedChunks = m_world.getLoadedChunks();

        for (auto it = m_chunkMeshData.begin(); it != m_chunkMeshData.end();) {
            if (loadedChunks.find(it->first) == loadedChunks.end()) {
                m_chunkMeshes.erase(it->first);
                it = m_chunkMeshData.erase(it);
            } else {
                ++it;
            }
        }

        for (auto& [pos, chunk] : loadedChunks) {
            if (!chunk.isMeshDirty()) continue;

            LayeredMeshData<BlockVertex> layeredMesh = ChunkMesher::buildMesh(
                m_assets.blockTextures(),
                m_blockRegistry,
                m_biomeRegistry,
                chunk
            );

#ifdef DEBUG
            std::cout << "[DEBUG] LayeredMesh for chunk (" << pos.x << ", " << pos.z << "): "
                      << "TotalVerts=" << layeredMesh.getTotalVertexCount()
                      << ", TotalIndices=" << layeredMesh.getTotalIndexCount()
                      << ", MemUsage=" << (layeredMesh.getMemoryUsage() / 1024.0f) << "KB"
                      << ", Empty=" << (layeredMesh.empty() ? "Y" : "N") << std::endl;

            layeredMesh.forEachNonEmptyLayer([&pos](Layer layer, const LayerData<BlockVertex>& data) {
                std::cout << "[DEBUG]   Layer" << static_cast<int>(layer) << ": "
                          << data.vertices.size() << "v, " << data.indices.size() << "i" << std::endl;
            });
#endif

            auto& meshData = m_chunkMeshData[pos] = std::move(layeredMesh);
            auto rawData = convertToRawMesh(meshData);

            auto [it, inserted] = m_chunkMeshes.try_emplace(pos, std::move(rawData), BlockVertex::getLayout());
            if (!inserted) {
                it->second.setVertexData(rawData.vertices.data(), rawData.vertices.size());
                it->second.setIndexData(rawData.indices.data(), rawData.indices.size());
            }
            auto& finalMesh = it->second;


#ifdef DEBUG
            std::cout << "[DEBUG] FinalMesh for chunk (" << pos.x << ", " << pos.z << "): "
                      << "Verts=" << finalMesh.getVertexCount()
                      << ", Indices=" << finalMesh.getIndexCount()
                      << ", Submeshes=" << finalMesh.getSubmeshCount()
                      << ", GPUMem=" << (finalMesh.getGPUMemoryUsage() / 1024.0f) << "KB"
                      << ", Empty=" << (finalMesh.isEmpty() ? "Y" : "N") << std::endl;
#endif


            chunk.markMeshClean();
        }
    }

    RawMeshData ChunkRenderSystem::convertToRawMesh(const LayeredMeshData<BlockVertex>& layeredData) {
        RawMeshData rawData;
        uint32_t vertexOffset = 0;

        for (size_t layerIdx = 0; layerIdx < static_cast<size_t>(Layer::LAYER_COUNT); ++layerIdx) {
            const auto currentLayer = static_cast<Layer>(layerIdx);
            const auto& layerData = layeredData.getLayer(currentLayer);

            if (layerData.empty()) continue;

            SubmeshInfo info;
            info.indexOffset = static_cast<uint32_t>(rawData.indices.size());
            info.indexCount = static_cast<uint32_t>(layerData.indices.size());
            info.baseVertex = vertexOffset;
            info.isEmpty = false;
            info.layer = currentLayer;

            const auto* vertexBytes = reinterpret_cast<const uint8_t*>(layerData.vertices.data());
            rawData.vertices.insert(rawData.vertices.end(), vertexBytes,
                                    vertexBytes + layerData.vertices.size() * sizeof(BlockVertex));

            for (auto idx : layerData.indices)
                rawData.indices.push_back(idx);

            vertexOffset += static_cast<uint32_t>(layerData.vertices.size());
            rawData.submeshes.push_back(info);
        }

        return rawData;
    }

    void ChunkRenderSystem::draw(Renderer& renderer, Registry& registry, float dt) {
        for (auto& [pos, mesh] : m_chunkMeshes) {
            for (size_t submeshIdx = 0; submeshIdx < mesh.getSubmeshCount(); ++submeshIdx) {
                const auto& submesh = mesh.getSubmesh(submeshIdx);
                if (submesh.isEmpty) continue;

                RenderCommand cmd{
                    &mesh,
                    m_assets.materials().getHandle("texture"),
                    pos.getModelMatrix(),
                    static_cast<uint32_t>(submeshIdx),
                    submesh.layer
                };
                renderer.submit(cmd);
            }
        }
    }
}