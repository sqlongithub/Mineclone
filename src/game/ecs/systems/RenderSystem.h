#pragma once
#include "ISystem.h"
#include "Renderer.h"

namespace Mineclone {
    class RenderSystem : public ISystem {
    public:
        explicit RenderSystem(Renderer& renderer);

        void setActiveCamera(Entity cameraEntity);
        //void update(Registry& registry, float dt) override;
        void draw(Registry &registry, float dt) override;

    private:
        Renderer &m_renderer;
        Entity m_activeCamera;


    };
}
