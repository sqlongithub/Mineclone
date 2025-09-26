#pragma once

#include "../renderer/Renderer.h"
#include "ecs/Registry.h"
#include "ecs/SystemManager.h"
#include "world/World.h"

namespace Mineclone {
    class Game {
    public:
        explicit Game(Registry& registry, SystemManager& systemManager, Dispatcher& dispatcher);
        void update(float dt);
        void draw(Renderer& renderer, float dt) const;
        void initSystems();
    private:
        void initAssets();

        // engine things
        AssetManager m_assetManager;
        EntityManager m_entityManager;

        SystemManager& m_systemManager;
        Registry& m_registry;
        Dispatcher m_dispatcher;

        // game things
        Entity m_camera;
        World m_world;
        BlockRegistry m_blockRegistry;
        BiomeRegistry m_biomeRegistry;




    };
}
