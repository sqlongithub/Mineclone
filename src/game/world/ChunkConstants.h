#pragma once

#include <cstdint>

namespace Mineclone {

    static constexpr int CHUNK_WIDTH = 16;
    static constexpr int CHUNK_DEPTH = 16;
    static constexpr int CHUNK_HEIGHT = 256;
    static constexpr size_t CHUNK_VOLUME = CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT;
}