#pragma once

#include "Shader.h"
#include "Texture.h"

namespace Mineclone {
    struct Material {
        Shader& shader;
        Texture* albedo = nullptr;
        Texture* normal = nullptr;
        glm::vec4 color = glm::vec4(1.0f);

    };
}