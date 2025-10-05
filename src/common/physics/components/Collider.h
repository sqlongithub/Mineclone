#pragma once
#include "common/core/math/AABB.h"

namespace Mineclone {

    struct Collider {
        AABB aabb;
        bool isDynamic = false;

        static Collider fullBlock() {
            return { AABB(glm::vec3(-0.5f), glm::vec3(0.5f)), false };
        }

        static Collider slab() {
            return { AABB(glm::vec3(-0.5f, -0.25f, -0.5f), glm::vec3(0.5f, 0.25f, 0.5f)), false };
        }

        static Collider player() {
            return { AABB(glm::vec3(-0.3f, 0.0f, -0.3f), glm::vec3(0.3f, 1.8f, 0.3f)), true };
        }
    };

}
