#pragma once
#include "common/ecs/Registry.h"
#include "common/ecs/SystemManager.h"
#include "rendering/Renderer.h"

#include "common/world/World.h"

namespace Mineclone {
    class Game {
    public:
        explicit Game(Registry& registry, SystemManager& systemManager, Window& window);
        void update(float dt);
        void draw(Renderer& renderer, float dt) const;
        void initSystems();
    private:
        void initAssets();

        // engine things
        AssetManager m_assetManager;
        EntityManager m_entityManager;
        Window& m_window;

        SystemManager& m_systemManager;
        Registry& m_registry;

        // game things
        Entity m_player;
        World m_world;
        BlockRegistry m_blockRegistry;
        BiomeRegistry m_biomeRegistry;




    };
}
