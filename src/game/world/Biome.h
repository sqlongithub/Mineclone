#pragma once

#include <string>
#include "glm/vec3.hpp"

namespace Mineclone {
    enum class BiomeType : uint8_t {
        Ocean = 0,
        Plains,
        Desert,
        Forest,
        Taiga,
        Mountains,
        Swamp,
        Jungle,
        Savanna,
        Tundra,
        COUNT
    };

    struct BiomeProperties {
        float baseTemperature = 0.5f;
        float baseHumidity = 0.5f;
        int surfaceLayerDepth = 1;
        int subLayerDepth = 4;

        std::string surfaceBlock = "grass";
        std::string subSurfaceBlock = "dirt";
        std::string stoneBlock = "stone";

        float heightVariation = 1.0f;
        int baseHeightOffset = 0;
        float treeChance = 0.02f;

        glm::vec3 grassTint = {1.0f, 1.0f, 1.0f};
        glm::vec3 foliageTint = {1.0f, 1.0f, 1.0f};
        glm::vec3 waterTint = {0.4f, 0.6f, 1.0f};
    };
}