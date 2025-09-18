#pragma once

#include <vector>
#include <cstdint>

namespace Mineclone {
    using BlockId = int;
    constexpr BlockId INVALID_BLOCK_ID = -1;
    constexpr BlockId AIR_BLOCK_ID = 0;

    struct Block {
        BlockId id;

        Block() = default;

        constexpr explicit Block(BlockId i) noexcept : id{i} {}
    };

    struct BlockMetadata {
        std::string name;
        std::string displayName;

        bool placeable = true;
        bool breakable = true;
        std::vector<std::string> tags;  // ["stone", "building_block"]

        std::string requiredTool;
        int minToolLevel = 0;           // Minimum tool tier needed

        std::string dropItem;
        int dropCount = 1;
        float dropChance = 1.0f;

        struct ExtraDrop {
            std::string item;
            int count;
            float chance;
        };
        std::vector<ExtraDrop> extraDrops;

        bool generateMipmaps = true;
        int lodDistance = 100;
    };
}