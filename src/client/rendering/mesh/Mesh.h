#pragma once

#include <vector>

#include "client/rendering/gpu/VertexArray.h"
#include "client/rendering/gpu/VertexLayout.h"
#include "client/rendering/render_queue/RenderLayer.h"

namespace Mineclone {

    struct SubmeshInfo {
        // in number of indices not bytes
        uint32_t indexOffset = 0;
        uint32_t indexCount = 0;
        uint32_t baseVertex = 0;
        bool isEmpty = true;
        Layer layer = Layer::TERRAIN;
    };

    struct RawMeshData {
        std::vector<uint8_t> vertices;  // raw bytes
        std::vector<uint32_t> indices;
        std::vector<SubmeshInfo> submeshes;
    };

    class Mesh {
    public:
        template<size_t N>
        Mesh(const RawMeshData& data, const VertexLayout<N>& layout)
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

        Mesh(Mesh&&) noexcept = default;
        Mesh& operator=(Mesh&&) noexcept = default;

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        ~Mesh();

        void setVertexData(const void* data, size_t size);
        void setIndexData(const void* data, size_t size, size_t offset = 0);

        [[nodiscard]] bool isEmpty() const { return m_totalVertexCount == 0; }
        [[nodiscard]] uint32_t getVertexCount() const { return m_totalVertexCount; }
        [[nodiscard]] uint32_t getIndexCount() const { return m_totalIndexCount; }
        [[nodiscard]] size_t getSubmeshCount() const { return m_submeshes.size(); }
        [[nodiscard]] GLenum getIndexType() const { return m_ibo.getType(); }

        [[nodiscard]] const VertexArray& getVAO() const { return m_vao; }
        [[nodiscard]] const std::vector<SubmeshInfo>& getSubmeshes() const { return m_submeshes; }
        [[nodiscard]] const SubmeshInfo& getSubmesh(size_t index) const { return m_submeshes[index]; }

        [[nodiscard]] size_t getGPUMemoryUsage() const;

    private:
        VertexArray m_vao;
        VertexBuffer m_vbo;
        IndexBuffer m_ibo;

        std::vector<SubmeshInfo> m_submeshes;
        uint32_t m_totalVertexCount = 0;
        uint32_t m_totalIndexCount = 0;

        template<size_t N>
        void setupVertexArray(const VertexLayout<N>& layout) {
            m_vao.bind();
            m_vao.addBuffer(m_vbo, layout);
            m_vao.addIndexBuffer(m_ibo);
            m_vao.unbind();
        }

        void validateMeshData(const RawMeshData& data) const;

#ifdef DEBUG
        bool validateSubmeshRanges() const;
#endif
    };

}
