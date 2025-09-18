
#include "DebugSystem.h"
#include "InputState.h"

namespace Mineclone {
    DebugSystem::DebugSystem(Dispatcher& dispatcher)
        : m_dispatcher(dispatcher) {

    }

    void DebugSystem::update(Registry &registry, float dt) {
        auto& input = registry.getSingleton<InputState>();
        if(input.isActionJustPressed(InputAction::DEBUG_TOGGLE_CURSOR)) {
            input.isCursorCaptured = !input.isCursorCaptured;
        }
        if(input.isActionJustPressed(InputAction::DEBUG_TOGGLE_WIREFRAME)) {
            input.isWireframeEnabled = !input.isWireframeEnabled;
            if(input.isWireframeEnabled) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
    }
}