#pragma once
#include <cstdint>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "client/rendering/gpu/VertexLayout.h"

namespace Mineclone {

    struct BlockVertex {
        glm::vec3 position{};
        glm::vec2 uv{};
        uint8_t faceIndex = 255;
        uint8_t textureIndex = 255;
        uint8_t tintIndex = 255;
        uint8_t padding = 0;
        float ao = 1.0f;

        static constexpr auto layout() {
            return Mineclone::VertexLayout<6>{
                std::array<Mineclone::VertexAttribute, 6>{
                    Mineclone::makeAttribute<BlockVertex, glm::vec3>(offsetof(BlockVertex, position)),
                    Mineclone::makeAttribute<BlockVertex, glm::vec2>(offsetof(BlockVertex, uv)),
                    Mineclone::makeAttribute<BlockVertex, uint8_t>(offsetof(BlockVertex, faceIndex)),
                    Mineclone::makeAttribute<BlockVertex, uint8_t>(offsetof(BlockVertex, textureIndex)),
                    Mineclone::makeAttribute<BlockVertex, uint8_t>(offsetof(BlockVertex, tintIndex)),
                    Mineclone::makeAttribute<BlockVertex, float>(offsetof(BlockVertex, ao))
                },
                sizeof(BlockVertex)
            };
        }
    };

}