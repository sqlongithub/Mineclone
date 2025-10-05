#pragma once

#include "glm/vec2.hpp"

namespace Mineclone {
    struct UVRect {
        glm::vec2 min, max;

        [[nodiscard]] glm::vec2 getCorner(int corner) const {
            switch(corner) {
                case 0: return {min.x, max.y}; // bottom-left
                case 1: return {max.x, max.y}; // bottom-right
                case 2: return {max.x, min.y}; // top-right
                case 3: return {min.x, min.y}; // top-left
                default: return min;
            }
        }
    };
}