#pragma once

#include <array>

#include <glaze/core/reflect.hpp>
#include "client/rendering/render_queue/RenderLayer.h"


namespace Mineclone {
    struct FaceDefinition {
        std::string texture;
        Layer layer = Layer::TERRAIN;
        int tintIndex = 255;
        std::optional<std::string> cullFace;
        std::array<float, 4> uv = {0,0,16,16};
    };

    struct BlockElement {
        glm::vec3 from = {0.0f, 0.0f, 0.0f}; // min corner
        glm::vec3 to   = {16.0f, 16.0f, 16.0f}; // max corner
        std::unordered_map<std::string, FaceDefinition> faces;

        BlockElement() = default;
        BlockElement(glz::make_reflectable) {};

        void expandFaceGroup(const std::string& groupName,
                        const std::initializer_list<const char*>& targetFaces) {
            const auto groupIt = faces.find(groupName);
            if (groupIt == faces.end()) {
                return;
            }

            const FaceDefinition& groupFace = groupIt->second;

            for (const char* faceName : targetFaces) {
                faces.try_emplace(faceName, groupFace);
            }

            faces.erase(groupIt);
        }
    };

    struct BlockMetadata {
        std::string name;
        std::string displayName;

        bool placeable = true;
        bool breakable = true;
        std::vector<std::string> tags{};  // ["stone", "building_block"]

        std::string requiredTool;
        int minToolLevel = 0;           // Minimum tool tier needed

        std::string dropItem;
        int dropCount = 1;
        float dropChance = 1.0f;

        bool solid = true;

        struct ExtraDrop {
            std::string item;
            int count;
            float chance;
        };
        std::vector<ExtraDrop> extraDrops{};

        bool generateMipmaps = true;
        int lodDistance = 100;
    };

    struct BlockModel {
        std::unordered_map<std::string, std::string> textures;
        std::vector<BlockElement> elements;

        BlockModel() = default;
        BlockModel(glz::make_reflectable) {}
    };

    struct BlockDefinition {
        BlockMetadata metadata;
        BlockModel model;

        BlockDefinition() = default;
        BlockDefinition(glz::make_reflectable) {}
    };
}

template <>
struct glz::meta<Layer> {
    using enum Layer;
    static constexpr auto value = enumerate(SKYBOX,
                                            TERRAIN,
                                            ENTITIES,
                                            TRANSPARENT,
                                            PARTICLES,
                                            UI,
                                            POST,
                                            LAYER_COUNT);
};

template <>
struct glz::meta<Mineclone::FaceDefinition> {
    static constexpr auto value = object(
            "texture", &Mineclone::FaceDefinition::texture,
            "layer", &Mineclone::FaceDefinition::layer,
            "tintindex", &Mineclone::FaceDefinition::tintIndex,
            "cullface", &Mineclone::FaceDefinition::cullFace,
            "uv", &Mineclone::FaceDefinition::uv
    );
};

template <>
struct glz::meta<Mineclone::BlockElement> {
    static constexpr auto value = object(
            "from", &Mineclone::BlockElement::from,
            "to", &Mineclone::BlockElement::to,
            "faces", &Mineclone::BlockElement::faces
    );
};

template <>
struct glz::meta<Mineclone::BlockMetadata::ExtraDrop> {
    static constexpr auto value = object(
            "item", &Mineclone::BlockMetadata::ExtraDrop::item,
            "count", &Mineclone::BlockMetadata::ExtraDrop::count,
            "chance", &Mineclone::BlockMetadata::ExtraDrop::chance
    );
};

template <>
struct glz::meta<Mineclone::BlockMetadata> {
    static constexpr auto value = object(
            "name", &Mineclone::BlockMetadata::name,
            "displayName", &Mineclone::BlockMetadata::displayName,
            "placeable", &Mineclone::BlockMetadata::placeable,
            "breakable", &Mineclone::BlockMetadata::breakable,
            "tags", &Mineclone::BlockMetadata::tags,
            "requiredTool", &Mineclone::BlockMetadata::requiredTool,
            "minToolLevel", &Mineclone::BlockMetadata::minToolLevel,
            "dropItem", &Mineclone::BlockMetadata::dropItem,
            "dropCount", &Mineclone::BlockMetadata::dropCount,
            "dropChance", &Mineclone::BlockMetadata::dropChance,
            "extraDrops", &Mineclone::BlockMetadata::extraDrops,
            "generateMipmaps", &Mineclone::BlockMetadata::generateMipmaps,
            "lodDistance", &Mineclone::BlockMetadata::lodDistance
    );
};

template <>
struct glz::meta<Mineclone::BlockModel> {
    static constexpr auto value = object(
            "textures", &Mineclone::BlockModel::textures,
            "elements", &Mineclone::BlockModel::elements
    );
};

template <>
struct glz::meta<Mineclone::BlockDefinition> {
    static constexpr auto value = object(
            "metadata", &Mineclone::BlockDefinition::metadata,
            "model", &Mineclone::BlockDefinition::model
    );
};