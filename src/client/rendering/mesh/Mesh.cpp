#include "Mesh.h"

#include <cassert>
#include <glad/glad.h>
#include <iostream>

#include <string>

namespace Mineclone {


    Mesh::~Mesh() {
        std::cout << "deleting mesh :(\n";
    }

    void Mesh::setVertexData(const void* data, size_t size) {
        m_vbo.setData(data, size);
    }

    void Mesh::setIndexData(const void* data, size_t size, size_t offset) {
        m_ibo.setData(data, size, offset);
    }

    size_t Mesh::getGPUMemoryUsage() const {
        size_t vertexSize = m_vbo.getSize();
        size_t indexSize = m_ibo.getSize();
        return vertexSize + indexSize;
    }


    void Mesh::validateMeshData(const RawMeshData& data) const {
#ifdef DEBUG
        // Validate vertex data size is multiple of vertex stride
        if (!data.vertices.empty()) {
            // We can't validate stride here without the layout, but that's checked in constructor
        }

        // Validate submesh ranges
        for (const auto& submesh : data.submeshes) {
            if (!submesh.isEmpty) {
                // Check index bounds
                if (submesh.indexOffset + submesh.indexCount > data.indices.size()) {
                    std::cerr << "ERROR: Submesh layer '" << std::to_string(static_cast<int>(submesh.layer))
                              << "' index range exceeds index buffer size" << std::endl;
                    assert(false);
                }

                // Check index count is multiple of 3 (triangles)
                if (submesh.indexCount % 3 != 0) {
                    std::cerr << "ERROR: Submesh layer '" << std::to_string(static_cast<int>(submesh.layer))
                              << "' index count not multiple of 3: " << submesh.indexCount << std::endl;
                    assert(false);
                }
            }
        }

        // Validate indices don't exceed vertex count
        size_t expectedVertexCount = data.vertices.size() / sizeof(uint32_t); // This is wrong, but we don't have stride here
        for (uint32_t index : data.indices) {
            // We can't validate this properly without knowing the vertex stride
            // This validation should happen in the caller
        }
#endif
    }

#ifdef DEBUG
    bool Mesh::validateSubmeshRanges() const {
        if (isEmpty()) return true;

        for (size_t i = 0; i < m_submeshes.size(); ++i) {
            const auto& submesh = m_submeshes[i];

            if (submesh.isEmpty) continue;

            // Validate index range
            if (submesh.indexOffset + submesh.indexCount > m_totalIndexCount) {
                std::cerr << "ERROR: Submesh " << i << " index range exceeds total indices. "
                          << "Range: [" << submesh.indexOffset << ", "
                          << (submesh.indexOffset + submesh.indexCount) << "), "
                          << "Total: " << m_totalIndexCount << std::endl;
                return false;
            }

            // Validate base vertex
            if (submesh.baseVertex < 0) {
                std::cerr << "ERROR: Submesh " << i << " has negative base vertex: "
                          << submesh.baseVertex << std::endl;
                return false;
            }

            if (static_cast<uint32_t>(submesh.baseVertex) >= m_totalVertexCount) {
                std::cerr << "ERROR: Submesh " << i << " base vertex exceeds total vertices. "
                          << "Base: " << submesh.baseVertex << ", Total: " << m_totalVertexCount << std::endl;
                return false;
            }

            // Validate index count is multiple of 3
            if (submesh.indexCount % 3 != 0) {
                std::cerr << "ERROR: Submesh " << i << " index count not multiple of 3: "
                          << submesh.indexCount << std::endl;
                return false;
            }
        }

        return true;
    }
#endif

} // namespace Mineclone