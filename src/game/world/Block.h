#pragma once

#include <vector>
#include <cstdint>

namespace Mineclone {
    using BlockId = int;
    constexpr BlockId INVALID_BLOCK_ID = -1;
    constexpr BlockId AIR_BLOCK_ID = 0;

    struct Block {
        BlockId id = AIR_BLOCK_ID;

        Block() = default;

        constexpr explicit Block(BlockId i) noexcept : id{i} {}
    };


}
