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

        void draw() const;

    private:
        VertexArray m_vertexArray;  // Vertex Array Object: stores vertex attribute bindings
        VertexBuffer m_vertexBuffer;  // Vertex Buffer Object: stores vertex data
        IndexBuffer m_indexBuffer;  // Element Buffer Object: stores index data
    };

}
