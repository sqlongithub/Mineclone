#pragma once

#include "glm/detail/type_mat4x4.hpp"

namespace Mineclone {
    struct FrameData {
        glm::mat4 viewProjection; // 4*4*4 = 64 bytes
        glm::vec3 cameraPosition; // 16 bytes because std140 for some reason
        float time; // 4 bytes
    };
}