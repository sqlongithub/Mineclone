
#include "CursorSystem.h"

#include "../components/InputState.h"

namespace Mineclone {
    CursorSystem::CursorSystem(Window& window)
        : m_window(window) {

    }

    void CursorSystem::update(Registry& registry, float dt) {
        auto& input = registry.getSingleton<InputState>();
        if(input.isCursorCaptured && !m_window.isCursorCaptured()) {
            m_window.captureCursor();
        } else if(!input.isCursorCaptured && m_window.isCursorCaptured()) {
            m_window.releaseCursor();
        }
    }
}
