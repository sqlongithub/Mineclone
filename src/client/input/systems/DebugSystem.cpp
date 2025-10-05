#include "DebugSystem.h"

#include "client/input/InputAction.h"
#include "client/input/components/InputState.h"
#include "common/player/components/Player.h"
#include "glad/glad.h"

namespace Mineclone {
    DebugSystem::DebugSystem() {

    }

    void DebugSystem::update(Registry &registry, float dt) {
        auto& input = registry.getSingleton<InputState>();
        if(input.isActionJustPressed(InputAction::DEBUG_TOGGLE_CURSOR)) {
            input.isCursorCaptured = !input.isCursorCaptured;
        }
        if(input.isActionJustPressed(InputAction::DEBUG_TOGGLE_WIREFRAME)) {
            input.isWireframeEnabled = !input.isWireframeEnabled;
            if(input.isWireframeEnabled) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }

        registry.view<PlayerState>([&](Entity e, PlayerState& ps) {
            if(input.isActionJustPressed(InputAction::DEBUG_TOGGLE_GAMEMODE)) {
                switch (ps.gameMode) {
                    case GameMode::CREATIVE:
                        ps.gameMode = GameMode::NOCLIP;
                        break;
                    case GameMode::NOCLIP:
                        ps.gameMode = GameMode::SURVIVAL;
                        break;
                    case GameMode::SURVIVAL:
                        ps.gameMode = GameMode::CREATIVE;
                        break;
                    default:
                        ps.gameMode = GameMode::NOCLIP;
                        break;
                }
            }
        });

    }
}
