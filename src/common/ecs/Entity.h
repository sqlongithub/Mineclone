#pragma once
#include <cstdint>

namespace Mineclone {
    using Entity = uint32_t;
    constexpr Entity INVALID_ENTITY = 0;

    class EntityManager {
    public:
        Entity create() { return m_nextID++; }

    private:
        uint32_t m_nextID = 1;
    };
}