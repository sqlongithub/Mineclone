#define GLM_ENABLE_EXPERIMENTAL
#include "imgui.h"
#include "client/rendering/Renderer.h"
#include "client/rendering/assets/AssetManager.h"
#include "client/rendering/components/Camera.h"
#include "common/ecs/Entity.h"
#include "common/ecs/Registry.h"
#include "common/ecs/components/Transform.h"
#include "RenderSystem.h"

#include "client/rendering/components/CameraController.h"
#include "client/rendering/components/Renderable.h"


namespace Mineclone {
    RenderSystem::RenderSystem(const AssetManager& assets)
        : m_assets(assets) {
    }

    void RenderSystem::setActiveCamera(Entity cameraEntity) {
        m_activeCamera = cameraEntity;
    }

    void RenderSystem::draw(Renderer& renderer, Registry& registry, float dt) {
        if (!m_activeCamera) return;

        auto& camT = registry.get<Transform>(m_activeCamera);
        auto& cam = registry.get<Camera>(m_activeCamera);
        auto& camController = registry.get<CameraController>(m_activeCamera);

        glm::mat4 view = cam.getViewMatrix(camT, camController.eyeHeight);

        registry.view<Renderable>([&](Entity e, Renderable& renderable) {
            const auto& transform = registry.get<Transform>(e);

            const RenderCommand cmd {
                .mesh = renderable.mesh,
                .material = renderable.material,
                .modelMatrix = transform.getMatrix()
            };

            renderer.submit(cmd);
        });

        ImGui::Begin("Camera Debug");
        auto fwd = camT.getForwardVector();
        if (ImGui::CollapsingHeader("Camera")) {
            ImGui::DragFloat3("Position", &camT.position.x, 0.1f);
            ImGui::DragFloat3("Direction", &fwd.x, 0.1f);
        }
        ImGui::End();
    }
}
