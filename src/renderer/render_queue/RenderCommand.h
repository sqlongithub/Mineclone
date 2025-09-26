#pragma once

#include "RenderLayer.h"
#include "glm/detail/type_mat4x4.hpp"
#include "renderer/mesh/Mesh.h"
#include "renderer/materials/Material.h"

namespace Mineclone {
    struct RenderCommand {
        Mesh* mesh = nullptr;
        Handle<Material> material;
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        uint32_t submeshIndex = UINT32_MAX;
        Layer layer = Layer::TERRAIN;

        [[nodiscard]] float getDistanceToCamera(const glm::vec3& cameraPos) const {
            auto objectPos = glm::vec3(modelMatrix[3]);
            return glm::length(cameraPos - objectPos);
        }
    };
}
