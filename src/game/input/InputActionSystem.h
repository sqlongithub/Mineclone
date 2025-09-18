#pragma once

#include <unordered_map>
#include "InputAction.h"
#include "Window.h"
#include "Registry.h"
#include "System.h"
#include "InputContextManager.h"

namespace Mineclone {
    class InputActionSystem : public System {
    public:
        InputActionSystem(const Window &window, InputContextManager& contextManager);

        // TODO: load from config
        void setDefaultBindings();

        void bindAction(InputAction action, int key);

        void bindAxis(InputAction action, MouseAxis axis);

        void update(Registry &registry, float dt) override; // Call once per frame

    private:
        const Window& m_window;
        const InputContextManager& m_contextManager;

        std::unordered_map <InputAction, std::vector<int>> m_actionBindings;
        std::unordered_map <InputAction, MouseAxis> m_axisBindings;


    };
}
