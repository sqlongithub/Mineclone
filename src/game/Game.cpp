#include "Game.h"
#include "Vertex.h"
#include "Mesh.h"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/string_cast.hpp"
#include "ecs/components/Transform.h"
#include "ecs/systems/InputActionSystem.h"
#include "ecs/systems/FreeCameraMovementSystem.h"
#include "RenderSystem.h"
#include "Renderable.h"
#include "RenderLayer.h"
#include "ImGuiRenderSystem.h"
#include "DebugSystem.h"
#include "CursorSystem.h"

namespace Mineclone {

    Game::Game(Registry& registry, SystemManager& systemManager, Dispatcher& dispatcher)
            : m_vertices({
                                 // position               color
                                 {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // 0: left-bottom-back (red)
                                 {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, // 1: right-bottom-back (green)
                                 {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}, // 2: right-top-back (blue)
                                 {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}}, // 3: left-top-back (yellow)
                                 {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f}}, // 4: left-bottom-front (magenta)
                                 {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f}}, // 5: right-bottom-front (cyan)
                                 {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}, // 6: right-top-front (white)
                                 {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}}, // 7: left-top-front (black)
                         }),
              m_indices({
                                // back face
                                0, 1, 2,
                                2, 3, 0,
                                // front face
                                4, 5, 6,
                                6, 7, 4,
                                // left face
                                0, 3, 7,
                                7, 4, 0,
                                // right face
                                1, 5, 6,
                                6, 2, 1,
                                // bottom face
                                0, 1, 5,
                                5, 4, 0,
                                // top face
                                3, 2, 6,
                                6, 7, 3
                        }),

              m_mesh(m_vertices, m_indices),
              m_entityManager(),
              m_systemManager(systemManager),
              m_registry(registry),
              m_shader("resources/shaders/basic.vert", "resources/shaders/basic.frag"),
              m_dispatcher(dispatcher)
    {
        m_camera = m_entityManager.create();
        m_registry.addEntity(m_camera, Transform {
                {0, 0, 0}
        }, Camera{});

        auto& camT = registry.get<Transform>(m_camera);
        auto& cam = registry.get<Camera>(m_camera);
        std::cout << "initial Camera pos = " << glm::to_string(camT.position) << std::endl;


        auto material = std::make_shared<Mineclone::Material>(m_shader, nullptr, nullptr, glm::vec4(1.0f));

        auto triangle = m_entityManager.create();
        m_registry.addEntity(triangle, Transform{{0,0,0}}, Renderable{ std::ref(m_mesh), std::ref(*material) }, Layer { Layer::TERRAIN });



    }

    void Game::initSystems() {
        m_systemManager.addSystem<FreeCameraMovementSystem>(m_dispatcher);
        m_systemManager.addSystem<DebugSystem>(m_dispatcher);

        m_systemManager.getSystem<RenderSystem>().setActiveCamera(m_camera);
    }

    void Game::update(float dt) {
        m_systemManager.updateAll(m_registry, dt);
    }

    void Game::draw(Renderer &renderer, float dt) const {
        m_systemManager.drawAll(m_registry, dt);
    }


}
