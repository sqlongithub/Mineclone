#pragma once

#include <unordered_map>
#include "InputAction.h"
#include "InputContextManager.h"
#include "core/Window.h"
#include "ecs/System.h"

namespace Mineclone {
    class InputActionSystem : public System {
    public:
        InputActionSystem(const Window &window, InputContextManager& contextManager);

        // TODO: load from config
        void setDefaultBindings();

        void bindAction(InputAction action, int key);

        void bindAxis(InputAction action, MouseAxis axis);

        void update(Registry &registry, float dt) override;

    private:
        const Window& m_window;
        const InputContextManager& m_contextManager;

        std::unordered_map <InputAction, std::vector<int>> m_actionBindings;
        std::unordered_map <InputAction, MouseAxis> m_axisBindings;


    };
}
