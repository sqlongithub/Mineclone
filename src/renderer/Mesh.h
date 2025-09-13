#pragma once

#include <vector>
#include "Vertex.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

namespace Mineclone {

    class Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        ~Mesh();
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        void draw() const;

        uint32_t getVertexCount();
        uint32_t getIndexCount();

    private:
        VertexArray m_vertexArray;  // Vertex Array Object: stores vertex attribute bindings
        VertexBuffer m_vertexBuffer;  // Vertex Buffer Object: stores vertex data
        IndexBuffer m_indexBuffer;  // Element Buffer Object: stores index data
        uint32_t m_vertexCount;
        uint32_t m_indexCount;
    };

}
