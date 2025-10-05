#pragma once

namespace Mineclone {
    struct CameraController {
        float yaw;
        float pitch;
        float lookSensitivity = 0.1f;
        float eyeHeight = 1.62f;
        float fov = glm::radians(80.0f);
        float baseFov = glm::radians(80.0f);
        float sprintFovScale = 1.1f;
        float fovLerpSpeed = 10.0f;
    };
}