#pragma once

#include "ISystem.h"
#include "Window.h"

namespace Mineclone {
    class CursorSystem : public ISystem {
    public:
        explicit CursorSystem(Window& window);
        void update(Mineclone::Registry &registry, float dt) override;
    private:
        Window& m_window;
        bool m_wasCursorCapturedLastUpdate = true;
    };

} // Mineclone
