#include "RenderSystem.h"
#include "Transform.h"
#include "Renderable.h"
#include "Camera.h"
#include "Renderer.h"
#include "glm/gtx/string_cast.hpp"
#include "RenderLayer.h"
#include "imgui.h"

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

        std::vector<Entity> layerBuckets[static_cast<size_t>(Layer::LAYER_COUNT)];

        registry.view<Renderable, RenderLayer>([&](Entity e, Renderable&, RenderLayer& layer){
            layerBuckets[static_cast<size_t>(layer.layer)].push_back(e);
        });

        glm::mat4 view = cam.getViewMatrix(camT);


        //std::cout << "Camera pos = " << glm::to_string(camT.position) << std::endl;
        //std::cout << "Camera rot = " << glm::to_string(camT.rotation) << std::endl;


        for (auto & layerBucket : layerBuckets) {
            for (Entity e : layerBucket) {
                auto& transform = registry.get<Transform>(e);
                auto& renderable = registry.get<Renderable>(e);

                RenderCommand cmd {
                    renderable.mesh,
                    renderable.material,
                    transform.getMatrix(),
                    Layer::ENTITIES,
                };

                renderer.submit(cmd);
            }
        }

        ImGui::Begin("Camera Debug");
        std::string title = "Camera";

        if (ImGui::CollapsingHeader(title.c_str())) {
            ImGui::DragFloat3("Position", &camT.position.x, 0.1f);
            ImGui::DragFloat3("Rotation", &camT.rotation.x, 0.1f);
        }
        ImGui::End();

    }
}
