#pragma once

#include "../renderer/Renderer.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Shader.h"
#include "ecs/components/Camera.h"
#include "ecs/Entity.h"
#include "ecs/Registry.h"
#include "ecs/SystemManager.h"
#include "ecs/systems/InputActionSystem.h"

namespace Mineclone {
    class Game {
    public:
        explicit Game(Registry& registry, SystemManager& systemManager, Dispatcher& dispatcher);
        void update(float dt);
        void draw(Renderer& renderer, float dt) const;
        void initSystems();
    private:
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;

        Mesh m_mesh;
        EntityManager m_entityManager;
        SystemManager& m_systemManager;
        Registry& m_registry;
        Entity m_camera;
        Shader m_shader;
        Dispatcher m_dispatcher;
    };
}
