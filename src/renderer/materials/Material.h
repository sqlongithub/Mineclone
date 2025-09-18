#pragma once

#include "Shader.h"
#include "Texture.h"
#include "assets/Handle.h"
#include "glm/vec4.hpp"
#include "TextureArray.h"

namespace Mineclone {
    struct Material {
        const Handle<Shader> shader;
        const TextureArray* textures;
        glm::vec4 color = glm::vec4(1.0f);

        bool isValid() const {
            return textures && shader.isValid();
        }
    };
}