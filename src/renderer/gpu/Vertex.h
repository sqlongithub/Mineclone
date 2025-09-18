#pragma once

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

typedef struct Vertex {
    glm::vec3 position;
    glm::vec3 color = {};
    glm::vec3 normal = {};
    glm::vec2 uv = {};
    unsigned short textureIndex = -1;
} Vertex;
