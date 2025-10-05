#pragma once

#include <ranges>
#include <vector>
#include "InputContext.h"

// Based on the state/context we are in, different actions should do different things.
// For example, menu up does something different in a chest gui than in a settings gui.

namespace Mineclone {
    class InputContextManager {
    public:
        void pushContext(const InputContext& ctx);
        void popContext();

        [[nodiscard]] InputAction mapAction(InputAction action) const;

    private:
        std::vector<InputContext> m_contextStack;

    };
}