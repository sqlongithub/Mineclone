#include "MoveIntentSystem.h"

#include "imgui.h"
#include "client/input/InputAction.h"
#include "client/input/components/InputState.h"
#include "client/rendering/components/Camera.h"
#include "common/ecs/components/Transform.h"
#include "common/physics/components/MoveIntent.h"
#include "common/player/components/Player.h"

namespace Mineclone {
    void MoveIntentSystem::update(Registry &registry, float dt) {
        auto& inputState = registry.getSingleton<InputState>();


        registry.view<MoveIntent, Transform, PlayerState>([&](Entity e, MoveIntent& intent, Transform& transform, PlayerState& playerState) {
            glm::vec3 forward = transform.getForwardVector();
            glm::vec3 right   = transform.getRightVector();

            forward.y = 0.0f;
            right.y   = 0.0f;

            if (glm::length(forward) > 0.001f) forward = glm::normalize(forward);
            if (glm::length(right) > 0.001f)   right = glm::normalize(right);

            glm::vec3 moveDir{0.0f};
            if (inputState.actionPressed[InputAction::MOVE_FORWARD])  moveDir += forward;
            if (inputState.actionPressed[InputAction::MOVE_BACKWARD]) moveDir -= forward;
            if (inputState.actionPressed[InputAction::MOVE_RIGHT])    moveDir += right;
            if (inputState.actionPressed[InputAction::MOVE_LEFT])     moveDir -= right;

            intent.direction = glm::length(moveDir) > 0.001f ? glm::normalize(moveDir) : glm::vec3{0.0f};
            intent.jump   = inputState.actionPressed[InputAction::JUMP];
            intent.sprint = inputState.actionPressed[InputAction::SPRINT];
            intent.crouch = inputState.actionPressed[InputAction::CROUCH];

            m_lastEntity = e;
        });


    }


    void MoveIntentSystem::draw(Renderer &renderer, Registry &registry, float dt) {
        auto& intent = registry.get<MoveIntent>(m_lastEntity);
        ImGui::Begin("Movement");
        if (ImGui::TreeNode("Intent")) {
            ImGui::DragFloat3("Direction", &intent.direction.x);
            ImGui::TreePop();
        }
        ImGui::End();
    }

}
