#pragma once

#include "glad/glad.h"

namespace Mineclone {
    inline constexpr size_t getGlEnumSize(GLenum type) {
        switch (type) {
            case GL_FLOAT:
                return sizeof(float);
            case GL_UNSIGNED_BYTE:
                return sizeof(unsigned char);
            case GL_INT:
                return sizeof(int);
            case GL_UNSIGNED_INT:
                return sizeof(unsigned int);
            default:
                return 0;
        }
    }

    class VertexAttribute {
    public:
        GLenum type;
        unsigned int count;
        size_t offset = 0;
        bool normalized = false;

        VertexAttribute() = default;

        VertexAttribute(GLenum t, unsigned int c, bool norm = false)
                : type(t), count(c), normalized(norm) {}

        [[nodiscard]] size_t getSize() const {
            return getGlEnumSize(type) * count;
        }

    };
}