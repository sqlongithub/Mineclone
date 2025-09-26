#include "InputContextManager.h"

namespace Mineclone {
    void InputContextManager::pushContext(const InputContext& ctx) {
        m_contextStack.push_back(ctx);
    }

    void InputContextManager::popContext() {
        if (!m_contextStack.empty())
            m_contextStack.pop_back();
    }

    InputAction InputContextManager::mapAction(InputAction action) const {
        for (const auto & it : std::ranges::reverse_view(m_contextStack)) {
            auto found = it.remap.find(action);
            if (found != it.remap.end())
                return found->second;
        }
        return action;
    }

}