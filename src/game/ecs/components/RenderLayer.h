#pragma once

enum Layer { SKYBOX, TERRAIN, ENTITIES, TRANSPARENT, PARTICLES, UI, POST, LAYER_COUNT };

struct RenderLayer {
    Layer layer;
};