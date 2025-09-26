#pragma once

#include <vector>

#include "renderer/gpu/VertexArray.h"
#include "renderer/gpu/VertexLayout.h"
#include "renderer/render_queue/RenderLayer.h"

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
        explicit Mesh(const RawMeshData& data, const VertexLayout& layout);

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

        void setupVertexArray(const VertexLayout& layout);
        void validateMeshData(const RawMeshData& data) const;

#ifdef DEBUG
        bool validateSubmeshRanges() const;
#endif
    };

}
