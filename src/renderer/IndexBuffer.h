#pragma once
#include <cstdint>

namespace Mineclone {
    class IndexBuffer {
    public:
        IndexBuffer(const void* data, uint32_t count);
        ~IndexBuffer();
        void bind() const;
        void unbind() const;

        [[nodiscard]] inline uint32_t getCount() const { return m_count; };
    private:
        uint32_t m_id;
        uint32_t m_count;

    };

} // Mineclone
