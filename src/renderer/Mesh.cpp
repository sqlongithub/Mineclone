#include <iostream>
#include "Mesh.h"
#include "glad/glad.h"

namespace Mineclone {
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
            : m_vertexArray(),
              m_vertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex)),
              m_indexBuffer(indices.data(), indices.size())
    {
        std::cout << "vertices size: " << vertices.size() << " indicies size: " << indices.size();
        VertexBufferLayout layout;
        layout.push<float>(3); // position
        layout.push<float>(3); // color
        layout.push<float>(3); // normal
        layout.push<float>(2); // uv

        m_vertexArray.addBuffer(m_vertexBuffer, layout);
        m_indexBuffer.bind();
        m_vertexArray.unbind();
    }

    Mesh::~Mesh() {

    }

    void Mesh::draw() const {
        m_vertexArray.bind();
        glDrawElements(GL_TRIANGLES, m_indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
        m_vertexArray.unbind();
        m_indexBuffer.unbind();
    }
}