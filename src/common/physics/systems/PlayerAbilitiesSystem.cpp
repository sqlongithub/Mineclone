#include "PlayerAbilitiesSystem.h"

#include "client/rendering/components/CameraController.h"
#include "common/physics/components/MoveIntent.h"
#include "common/player/components/Player.h"

namespace Mineclone {

    void PlayerAbilitiesSystem::update(Registry &registry, float dt) {
        registry.view<MoveIntent, PlayerState, CameraController>([&](Entity e, MoveIntent& moveIntent, PlayerState& playerState, CameraController& cameraController) {
            bool jumpPressed = moveIntent.jump;
            bool& wasJumping = m_wasJumping[e];

            // only run when jump just got pressed
            if (playerState.canToggleFly() && jumpPressed && !wasJumping) {
                float currentTime = m_totalTime;
                auto it = m_lastJumpTime.find(e);

                if (it != m_lastJumpTime.end()) {
                    float timeSinceLastJump = currentTime - it->second;
                    if (timeSinceLastJump < 0.3f) {
                        playerState.isFlying = !playerState.isFlying;
                        m_lastJumpTime.erase(it);
                    } else {
                        it->second = currentTime;
                    }
                } else {
                    m_lastJumpTime[e] = currentTime;
                }
            }

            wasJumping = jumpPressed;
            playerState.isSprinting = moveIntent.sprint;
        });

        m_totalTime += dt;

    }


}