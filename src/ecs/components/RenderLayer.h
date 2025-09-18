#pragma once

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

struct RenderLayer {
    Layer layer;
};