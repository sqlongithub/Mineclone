#pragma once
#include <array>
#include <span>
#include <cstddef>
#include <type_traits>
#include "VertexAttribute.h"
#include "GLTypeTraits.h"

namespace Mineclone {

    template<size_t N>
    class VertexLayout {
    public:
        constexpr VertexLayout(std::array<VertexAttribute, N> attrs, size_t stride)
            : m_attributes(attrs), m_stride(stride) {}

        constexpr std::span<const VertexAttribute> getAttributes() const {
            return m_attributes;
        }

        constexpr size_t getStride() const { return m_stride; }
        constexpr size_t getAttributeCount() const { return N; }

    private:
        std::array<VertexAttribute, N> m_attributes;
        size_t m_stride;
    };

    template<typename Vertex, typename Member>
    consteval VertexAttribute makeAttribute(size_t offset, bool normalized = false) {
        using MemberType = std::remove_cv_t<Member>;

        return VertexAttribute{
            GLTypeTraits<MemberType>::type,
            GLTypeTraits<MemberType>::count,
            offset,
            normalized,
            GLTypeTraits<MemberType>::isInteger
        };
    }

    template<typename Vertex, size_t N>
    consteval auto makeVertexLayout(const std::array<VertexAttribute, N>& attrs) {
        return VertexLayout<N>{attrs, sizeof(Vertex)};
    }

}