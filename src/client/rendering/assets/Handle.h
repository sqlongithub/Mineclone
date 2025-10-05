#pragma once

#include <cstdint>

namespace Mineclone {
    template<typename T>
    struct Handle {
        uint32_t index : 16;     // max 16M resources per type
        uint32_t generation : 16; // validation

        constexpr Handle() : index(0), generation(0) {}
        constexpr Handle(uint32_t idx, uint32_t gen) : index(idx), generation(gen) {}

        [[nodiscard]] bool isValid() const { return index != 0; }
        static constexpr Handle invalid() { return Handle(); }

        constexpr bool operator==(const Handle& other) const {
            return index == other.index && generation == other.generation;
        }
        constexpr bool operator!=(const Handle& other) const {
            return *this != other;
        }
    };
}