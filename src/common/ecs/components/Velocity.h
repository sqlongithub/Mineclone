#pragma once
#include "glm/vec3.hpp"

namespace Mineclone {
    struct Velocity {
        glm::vec3 velocity = glm::vec3(0.0f);
        glm::vec3 acceleration = glm::vec3(0.0f, -9.81f, 0.0f);

        float maxSpeed = 50.0f;
    };
}
