#pragma once
#include <cstdint>

#include "glm/vec2.hpp"
#include "renderer/gpu/VertexLayout.h"

namespace Mineclone {

    struct BlockVertex {
        glm::vec3 position {};
        glm::vec2 uv {};
        uint8_t faceIndex = 255;
        uint8_t textureIndex = 255;
        uint8_t tintIndex = 255;
        uint8_t padding = 0;

        static inline VertexLayout getLayout() {
            static const VertexLayout layout = VertexLayout::create({
                {GL_FLOAT, 3},                    // position (3 floats)
                {GL_FLOAT, 2},                    // uv (2 floats)
                {GL_UNSIGNED_BYTE, 1},            // faceIndex (1 byte)
                {GL_UNSIGNED_BYTE, 1},            // textureIndex (1 byte)
                {GL_UNSIGNED_BYTE, 1},            // tintIndex (1 byte)
                {GL_UNSIGNED_BYTE, 1}
            });
            return layout;
        }
    };



}
