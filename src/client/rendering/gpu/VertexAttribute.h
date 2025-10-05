#pragma once
#include <cstddef>
#include "glad/glad.h"

namespace Mineclone {

    struct VertexAttribute {
        GLenum type;
        int count;
        size_t offset;
        bool normalized;
        bool isInteger;

        constexpr VertexAttribute(GLenum t, int c, size_t o, bool norm, bool isInt)
            : type(t), count(c), offset(o), normalized(norm), isInteger(isInt) {}
    };

}
