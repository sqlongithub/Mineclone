#pragma once

#include <string>
#include <glaze/core/reflect.hpp>
#include <glaze/json/write.hpp>
#include <glaze/json/read.hpp>
#include <glaze/json/json_t.hpp>
#include "glm/vec3.hpp"


namespace Mineclone {
    using BiomeId = int;
    static constexpr BiomeId INVALID_BIOME_ID = -1;
    static constexpr uint8_t NO_TINT_INDEX = 255;

    struct BiomeTerrainParameters {
        float temperature;
        float humidity;
        float continentalness;
        float erosion;
        float weirdness;
    };

    struct BiomeMetadata {
        //explicit BiomeMetadata() = default;

    //    BiomeMetadata(glz::make_reflectable) {}

        std::string name;
        std::string displayName;
        std::vector<std::string> tags;

        BiomeTerrainParameters terrainParameters;

        glm::vec3 grassTint = {0.569f, 0.741f, 0.349f};
        glm::vec3 foliageTint = {1.0f, 1.0f, 1.0f};
        glm::vec3 waterTint = {0.4f, 0.6f, 1.0f};
        glm::vec3 skyTint = {0.5f, 0.7f, 1.0f};

        float fogStart = 96.0f;
        float fogEnd = 128.0f;
        glm::vec3 fogColor = {0.8f, 0.9f, 1.0f};
    };


}

namespace glz {

    template <>
    struct meta<Mineclone::BiomeTerrainParameters> {
        static constexpr auto value = object(
                "temperature", &Mineclone::BiomeTerrainParameters::temperature,
                "humidity", &Mineclone::BiomeTerrainParameters::humidity,
                "continentalness", &Mineclone::BiomeTerrainParameters::continentalness,
                "erosion", &Mineclone::BiomeTerrainParameters::erosion,
                "weirdness", &Mineclone::BiomeTerrainParameters::weirdness
        );
    };

    template <>
    struct meta<Mineclone::BiomeMetadata> {
        static constexpr auto value = object(
                "name", &Mineclone::BiomeMetadata::name,
                "displayName", &Mineclone::BiomeMetadata::displayName,
                "tags", &Mineclone::BiomeMetadata::tags,
                "terrain", &Mineclone::BiomeMetadata::terrainParameters,
                "grassTint", &Mineclone::BiomeMetadata::grassTint,
                "foliageTint", &Mineclone::BiomeMetadata::foliageTint,
                "waterTint", &Mineclone::BiomeMetadata::waterTint,
                "skyTint", &Mineclone::BiomeMetadata::skyTint,
                "fogStart", &Mineclone::BiomeMetadata::fogStart,
                "fogEnd", &Mineclone::BiomeMetadata::fogEnd,
                "fogColor", &Mineclone::BiomeMetadata::fogColor
        );
    };
}



