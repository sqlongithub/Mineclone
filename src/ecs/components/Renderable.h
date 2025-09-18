#pragma once

#include "mesh/Mesh.h"
#include "materials/Material.h"

namespace Mineclone {
    struct Renderable {
        Mesh* mesh = nullptr;
        Handle<Material> material;
    };
}