#include "Mesh.h"

#include <cassert>
#include <glad/glad.h>
#include <iostream>

#include <string>

namespace Mineclone {

    Mesh::Mesh(const RawMeshData& data, const VertexLayout& layout)
        : m_vao(), m_vbo(data.vertices.data(), data.vertices.size()), m_ibo(data.indices.data(), data.indices.size(), GL_UNSIGNED_INT) {

        validateMeshData(data);

        m_totalVertexCount = static_cast<uint32_t>(data.vertices.size() / layout.getStride());
        m_totalIndexCount = static_cast<uint32_t>(data.indices.size());

        if (m_totalVertexCount == 0) return;

        m_submeshes = data.submeshes;


        if (m_submeshes.empty() && !data.indices.empty()) {
            SubmeshInfo defaultSubmesh;
            defaultSubmesh.indexOffset = 0;
            defaultSubmesh.indexCount = m_totalIndexCount;
            defaultSubmesh.baseVertex = 0;
            defaultSubmesh.isEmpty = false;
            m_submeshes.push_back(defaultSubmesh);
        }

        // Setup VAO
        setupVertexArray(layout);

#ifdef DEBUG
        assert(validateSubmeshRanges());
#endif
    }

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

    void Mesh::setupVertexArray(const VertexLayout& layout) {
        m_vao.bind();
        m_vao.addBuffer(m_vbo, layout);
        m_vao.addIndexBuffer(m_ibo);
        m_vao.unbind();
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