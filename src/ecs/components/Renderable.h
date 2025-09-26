#pragma once

namespace Mineclone {
    struct Renderable {
        Mesh* mesh = nullptr;
        Handle<Material> material;
    };
}