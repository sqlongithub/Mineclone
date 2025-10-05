#pragma once
#include <cstdint>

namespace Mineclone {
    enum class GameMode {
        SURVIVAL,
        CREATIVE,
        NOCLIP,
    };

    struct PlayerIdentity {
        uint16_t id = 0;
        bool isLocal = false;
    };

    struct PlayerState {
        GameMode gameMode = GameMode::SURVIVAL;
        bool isFlying = false;
        bool isSprinting = false;
        bool isCrouching = false;
        float lastGrounded = 0.0f;
        bool isGrounded = false;




        [[nodiscard]] bool canToggleFly() const {
            return gameMode == GameMode::CREATIVE;
        }
        [[nodiscard]] bool hasCollision() const {
            return gameMode != GameMode::NOCLIP;
        }
        [[nodiscard]] bool isInvincible() const {
            return gameMode == GameMode::CREATIVE || gameMode == GameMode::NOCLIP;
        }
    };
}
