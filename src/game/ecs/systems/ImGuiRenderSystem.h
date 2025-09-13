#pragma once

#include "ISystem.h"

namespace Mineclone {
    class ImGuiRenderSystem : public ISystem {
    public:
        ImGuiRenderSystem();
        void update(Registry& registry, float dt) override;
        void draw(Registry& registry, float dt) override;
    private:
    };
}
