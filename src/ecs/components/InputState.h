#pragma once

#include <optional>
#include "input/InputAction.h"
#include "Entity.h"

namespace Mineclone {
    struct InputState {
        // Action states
        std::unordered_map<InputAction, float> actionValues;
        std::unordered_map<InputAction, bool> actionPressed;
        std::unordered_map<InputAction, bool> actionJustPressed;

        // Mouse delta
        float mouseDeltaX = 0.0f;
        float mouseDeltaY = 0.0f;

        // Last known mouse position
        float lastMouseX = 0.0f;
        float lastMouseY = 0.0f;

        bool isCursorCaptured;
        bool isWireframeEnabled;

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