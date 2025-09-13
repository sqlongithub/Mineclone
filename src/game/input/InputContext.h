#pragma once

#include <unordered_map>
#include "InputAction.h"

namespace Mineclone {
    enum class InputContextType {
        GAMEPLAY,
        MAIN_MENU,
        PAUSE_MENU,
        DEBUG,
    };

    struct InputContext {
        InputContextType type;
        std::unordered_map<InputAction, InputAction> remap;
    };
}