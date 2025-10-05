#pragma once
#include "glm/vec3.hpp"

namespace Mineclone {
    struct MoveIntent {
        glm::vec3 direction = glm::vec3(0.0f);
        bool jump = false;
        bool sprint = false;
        bool crouch = false;
    };
}
