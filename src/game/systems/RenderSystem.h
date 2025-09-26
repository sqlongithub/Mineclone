#pragma once

#include "ecs/System.h"
#include "renderer/Renderer.h"
#include "renderer/assets/AssetManager.h"

namespace Mineclone {
    class RenderSystem : public System {
    public:
        explicit RenderSystem(const AssetManager& assets);

        void setActiveCamera(Entity cameraEntity);
        //void update(Registry& registry, float dt) override;
        void draw(Renderer& renderer, Registry &registry, float dt) override;

    private:
        const AssetManager& m_assets;
        Entity m_activeCamera;


    };
}
