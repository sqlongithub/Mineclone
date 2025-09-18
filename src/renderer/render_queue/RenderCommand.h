#pragma once

#include "mesh/Mesh.h"
#include "materials/Material.h"
#include "RenderLayer.h"
#include "glm/detail/type_mat4x4.hpp"
#include "math/AABB.h"

namespace Mineclone {
    struct RenderCommand {
        Mesh* mesh = nullptr;            // pointer to mesh (chunk or entity)
        Handle<Material> material;    // Material handle in asset manager
        glm::mat4 modelMatrix = glm::mat4(1.0f); // transform for this object
        Layer layer = Layer::TERRAIN; // which layer to draw in

        [[nodiscard]] float getDistanceToCamera(const glm::vec3& cameraPos) const {
            auto objectPos = glm::vec3(modelMatrix[3]);
            return glm::length(cameraPos - objectPos);
        }
    };
}