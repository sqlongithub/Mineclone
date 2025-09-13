#pragma once

#include "Mesh.h"
#include "Material.h"

namespace Mineclone {
    struct Renderable {
        std::reference_wrapper<Mesh> mesh;
        std::reference_wrapper<Material> material;
    };
}