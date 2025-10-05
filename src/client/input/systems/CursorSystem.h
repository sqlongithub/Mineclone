#pragma once
#include "common/ecs/System.h"

namespace Mineclone {
    class Window;

    class CursorSystem : public System {
    public:
        explicit CursorSystem(Window& window);
        void update(Registry& registry, float dt) override;
    private:
        Window& m_window;
    };

}
