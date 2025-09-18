#pragma once

#include <vector>
#include "gpu/Vertex.h"
#include "gpu/IndexBuffer.h"
#include "gpu/VertexBuffer.h"
#include "gpu/VertexArray.h"
#include "math/AABB.h"

namespace Mineclone {

    class Mesh {
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        ~Mesh() = default;
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        Mesh(Mesh&&) = default;
        Mesh& operator=(Mesh&&) = default;

        [[nodiscard]] const AABB& getBoundingBox() const { return m_boundingBox; }

        uint32_t getVertexCount();
        uint32_t getIndexCount();
        [[nodiscard]] GLenum getIndexType() const { return m_indexBuffer.getType(); };
        [[nodiscard]] const VertexArray& getVAO() const { return m_vertexArray; };


    private:
        VertexArray m_vertexArray;
        VertexBuffer m_vertexBuffer;
        IndexBuffer m_indexBuffer;
        uint32_t m_vertexCount;
        uint32_t m_indexCount;

        AABB m_boundingBox;
    };

}
