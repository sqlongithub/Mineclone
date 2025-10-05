#pragma once
#include "common/ecs/System.h"

namespace Mineclone {
    class ImGuiRenderSystem : public System {
    public:
        ImGuiRenderSystem();
        void update(Registry& registry, float dt) override;
        void draw(Renderer& renderer, Registry& registry, float dt) override;
    private:
    };
}
