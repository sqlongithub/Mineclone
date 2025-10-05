#pragma once
#include <cstdint>
#include <cstddef>

enum class Layer : uint8_t {
    SKYBOX,
    TERRAIN,
    ENTITIES,
    TRANSPARENT,
    PARTICLES,
    UI,
    POST,
    LAYER_COUNT
};

constexpr std::size_t LAYER_COUNT = static_cast<std::size_t>(Layer::LAYER_COUNT);

struct RenderLayer {
    Layer layer;
};