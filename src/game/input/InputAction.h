#pragma once

namespace Mineclone {
    enum class InputAction {
        MOVE_FORWARD,
        MOVE_BACKWARD,
        MOVE_LEFT,
        MOVE_RIGHT,
        SPRINT,
        JUMP,
        LOOK_HORIZONTAL,
        LOOK_VERTICAL,
        INTERACT,
        ATTACK,
        MENU,
        DEBUG_TOGGLE_CURSOR,
        DEBUG_TOGGLE_WIREFRAME,
    };

    enum class MouseAxis {
        X, Y
    };
}