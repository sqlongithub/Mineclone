#pragma once

#include <unordered_map>

#include "client/input/InputAction.h"

namespace Mineclone {
    struct InputState {
        std::unordered_map<InputAction, float> actionValues;
        std::unordered_map<InputAction, bool> actionPressed;
        std::unordered_map<InputAction, bool> actionJustPressed;

        float mouseDeltaX = 0.0f;
        float mouseDeltaY = 0.0f;

        float lastMouseX = 0.0f;
        float lastMouseY = 0.0f;

        bool isCursorCaptured = false;
        bool isWireframeEnabled = false;


        InputState() = default;

        InputState(const InputState& other) = default;

        InputState(InputState&& other) noexcept = default;

        InputState& operator=(const InputState& other) = default;

        InputState& operator=(InputState&& other) noexcept = default;

        bool isActionPressed(InputAction action) const {
            auto it = actionPressed.find(action);
            return it != actionPressed.end() && it->second;
        }

        bool isActionJustPressed(InputAction action) const {
            auto it = actionJustPressed.find(action);
            return it != actionJustPressed.end() && it->second;
        }

        float getActionValue(InputAction action) const {
            auto it = actionValues.find(action);
            return it != actionValues.end() ? it->second : 0.0f;
        }

        float getMouseDeltaX() const { return mouseDeltaX; }
        float getMouseDeltaY() const { return mouseDeltaY; }
    };
}