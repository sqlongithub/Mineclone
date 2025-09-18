#pragma once

#include <cstdint>

namespace Mineclone {
    class UniformBuffer {
    public:
        UniformBuffer(size_t size, uint32_t binding);
        ~UniformBuffer();

        void setData(const void* data, size_t size, size_t offset = 0);

        void bind() const;
        void unbind() const;
    private:
        unsigned int m_id;
        uint32_t m_bindingPoint;
        std::size_t m_size;

    };
}
