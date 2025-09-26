#pragma once

#include "VertexAttribute.h"
#include <vector>

namespace Mineclone {
    class VertexLayout {
    public:
        static VertexLayout create(std::initializer_list<VertexAttribute> attrs) {
            VertexLayout layout;
            size_t offset = 0;
            for (auto attr : attrs) {
                attr.offset = offset;
                layout.m_attributes.push_back(attr);
                offset += attr.getSize();
            }
            layout.m_stride = offset;
            return layout;
        }

        [[nodiscard]] const std::vector<VertexAttribute>& getAttributes() const { return m_attributes; }
        [[nodiscard]] size_t getStride() const { return m_stride; }

    private:
        std::vector<VertexAttribute> m_attributes;
        size_t m_stride = 0;
    };


}
