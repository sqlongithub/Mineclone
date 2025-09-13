
#include "CursorSystem.h"
#include "InputData.h"

namespace Mineclone {
    CursorSystem::CursorSystem(Window &window)
        : m_window(window),
          m_wasCursorCapturedLastUpdate(m_window.isCursorCaptured()) {

    }

    void CursorSystem::update(Mineclone::Registry &registry, float dt) {
        auto& input = registry.getSingleton<InputState>();
        if(input.isCursorCaptured && !m_window.isCursorCaptured()) {
            m_window.captureCursor();
        } else if(!input.isCursorCaptured && m_window.isCursorCaptured()) {
            m_window.releaseCursor();
        }
    }
}