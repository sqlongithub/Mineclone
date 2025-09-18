#pragma once

#include <array>
#include <glaze/core/reflect.hpp>

namespace Mineclone {
    struct BlockModel {
        BlockModel() = default;

        static constexpr int INVALID_TEXTURE_INDEX = -1;

        std::array<int, 6> faceTextureIndices;

        BlockModel(glz::make_reflectable) {}

        explicit BlockModel(int allIndex)
                : faceTextureIndices({allIndex}) {

        }

        BlockModel(int topIndex, int bottomIndex, int sideIndex)
                : faceTextureIndices({topIndex, bottomIndex, sideIndex, sideIndex, sideIndex, sideIndex}) {

        }

        BlockModel(int topIndex, int bottomIndex, int northIndex, int southIndex, int eastIndex, int westIndex)
                : faceTextureIndices({topIndex, bottomIndex, northIndex, southIndex, eastIndex, westIndex}) {

        }

    };

    struct BlockJson {
        std::string name;
        std::unordered_map<std::string, std::string> textures;
    };


}

template <>
struct glz::meta<Mineclone::BlockJson> {
    static constexpr auto value = object(
            "name", &Mineclone::BlockJson::name,
            "textures", &Mineclone::BlockJson::textures
    );
};