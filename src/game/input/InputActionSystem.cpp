#include "InputActionSystem.h"
#include "InputAction.h"
#include "ecs/components/InputState.h"
#include "GLFW/glfw3.h"


namespace Mineclone {

    InputActionSystem::InputActionSystem(const Window& window, InputContextManager& contextManager)
            : m_window(window),
              m_contextManager(contextManager) {
        setDefaultBindings();
    }

    void InputActionSystem::setDefaultBindings() {
        bindAction(InputAction::MOVE_FORWARD, GLFW_KEY_W);
        bindAction(InputAction::MOVE_BACKWARD, GLFW_KEY_S);
        bindAction(InputAction::MOVE_LEFT, GLFW_KEY_A);
        bindAction(InputAction::MOVE_RIGHT, GLFW_KEY_D);
        bindAction(InputAction::SPRINT, GLFW_KEY_LEFT_CONTROL);

        bindAction(InputAction::JUMP, GLFW_KEY_SPACE);
        bindAction(InputAction::INTERACT, GLFW_KEY_E);
        bindAction(InputAction::ATTACK, GLFW_MOUSE_BUTTON_LEFT);
        bindAction(InputAction::MENU, GLFW_KEY_ESCAPE);
        bindAction(InputAction::DEBUG_TOGGLE_CURSOR, GLFW_KEY_F3);
        bindAction(InputAction::DEBUG_TOGGLE_WIREFRAME, GLFW_KEY_F4);


        bindAxis(InputAction::LOOK_HORIZONTAL, MouseAxis::X);
        bindAxis(InputAction::LOOK_VERTICAL, MouseAxis::Y);
    }

    void InputActionSystem::bindAction(InputAction action, int key) {
        m_actionBindings[action].push_back(key);
    }

    void InputActionSystem::bindAxis(InputAction action, MouseAxis axis) {
        m_axisBindings[action] = axis;
    }

    void InputActionSystem::update(Registry& registry, float dt) {
        auto& inputState = registry.getSingleton<InputState>();

        inputState.actionJustPressed.clear();

        auto [mouseX, mouseY] = m_window.getCurrentMousePosition();
        inputState.mouseDeltaX = mouseX - inputState.lastMouseX;
        inputState.mouseDeltaY = mouseY - inputState.lastMouseY;
        inputState.lastMouseX = mouseX;
        inputState.lastMouseY = mouseY;

        for (const auto& [action, keys] : m_actionBindings) {
            InputAction mapped = m_contextManager.mapAction(action);

            bool wasPressed = inputState.actionPressed[mapped];
            bool isPressed = false;

            for (int key : keys) {
                if (m_window.isKeyPressed(key)) {
                    isPressed = true;
                    break;
                }
            }

            inputState.actionPressed[mapped] = isPressed;
            inputState.actionJustPressed[mapped] = isPressed && !wasPressed;
            inputState.actionValues[mapped] = isPressed ? 1.0f : 0.0f;
        }

        for (const auto& [action, axis] : m_axisBindings) {
            float value = 0.0f;
            switch (axis) {
                case MouseAxis::X: value = inputState.mouseDeltaX; break;
                case MouseAxis::Y: value = inputState.mouseDeltaY; break;
            }

            inputState.actionValues[action] = value;

            bool hasMovement = std::abs(value) > 0.001f;
            bool wasPressed = inputState.actionPressed[action];

            inputState.actionPressed[action] = hasMovement;
            inputState.actionJustPressed[action] = hasMovement && !wasPressed;
        }

    }



}