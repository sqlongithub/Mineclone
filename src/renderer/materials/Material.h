#pragma once

#include "TextureArray.h"
#include "MaterialParameters.h"
#include "renderer/assets/Handle.h"

namespace Mineclone {

    struct Material {
        const Handle<Shader> shader;
        const TextureArray* textures;
        MaterialParameters parameters;

        glm::vec4 color = glm::vec4(1.0f);

        bool isValid() const {
            return textures && shader.isValid();
        }
    };
}
