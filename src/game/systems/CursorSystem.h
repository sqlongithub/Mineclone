#pragma once

#include "System.h"
#include "Window.h"
#include "Renderer.h"

namespace Mineclone {
    class CursorSystem : public System {
    public:
        explicit CursorSystem(Window& window);
        void update(Registry& registry, float dt) override;
    private:
        Window& m_window;
    };

} // Mineclone
