#include "Game.h"
#include "Vertex.h"
#include "Mesh.h"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

namespace Mineclone {


    Game::Game()
            : m_vertices({
                                 //   position           color
                                 {{ 0.0f,  0.5f, -1.0f}, {1.0f, 0.0f, 0.0f}},  // top vertex (red)
                                 {{-0.5f, -0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}},  // bottom-left (green)
                                 {{ 0.5f, -0.5f, -1.0f}, {0.0f, 0.0f, 1.0f}},  // bottom-right (blue)
                         }),
              m_indices({
                                0, 1, 2   // single triangle
                        }),
              m_mesh(m_vertices, m_indices),
              m_shader("resources/shaders/basic.vert", "resources/shaders/basic.frag")
    {

    }


    void Game::update(float dt) {

    }

    void Game::draw(const Renderer &renderer) const {
        m_shader.bind();

        // Model (identity, triangle stays at origin)
        auto model = glm::mat4(1.0f);

        // View (camera at (0,0,3) looking at origin, up = +Y)
        glm::mat4 view = glm::lookAt(
                glm::vec3(0.0f, 0.0f, 3.0f),  // camera position
                glm::vec3(0.0f, 0.0f, 0.0f),  // target
                glm::vec3(0.0f, 1.0f, 0.0f)   // up
        );

        // Projection (perspective)
        glm::mat4 projection = glm::perspective(
                glm::radians(45.0f),          // FOV
                1280.0f / 720.0f,             // aspect ratio
                0.1f,                         // near plane
                100.0f                        // far plane
        );

        glm::mat4 mvp = projection * view * model;

        // Upload to shader
        m_shader.setUniformMat4("u_MVP", mvp);

        m_mesh.draw();
    }


}
