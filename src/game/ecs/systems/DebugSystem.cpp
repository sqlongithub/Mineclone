
#include "DebugSystem.h"
#include "InputData.h"

namespace Mineclone {
    DebugSystem::DebugSystem(Dispatcher& dispatcher)
        : m_dispatcher(dispatcher) {

    }

    void DebugSystem::update(Registry &registry, float dt) {
        auto& input = registry.getSingleton<InputState>();
        if(input.isActionJustPressed(InputAction::DEBUG_TOGGLE_CURSOR)) {
            input.isCursorCaptured = false;
        }
    }
}