#pragma once

#include "../renderer/Renderer.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Shader.h"

namespace Mineclone {
    class Game {
    public:
        explicit Game();
        void update(float dt);
        void draw(const Renderer& renderer) const;
    private:
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        Mesh m_mesh;
        Shader m_shader;
    };
}
