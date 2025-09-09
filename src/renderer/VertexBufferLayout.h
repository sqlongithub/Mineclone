#pragma once

#include "glad/glad.h"
#include <cstdint>
#include <vector>
#include <stdexcept>


namespace Mineclone {

    struct VertexBufferElement {
        unsigned int type;
        unsigned int count;
        bool normalized;

        static inline unsigned int getSizeOfGLType(unsigned int type) {
            switch (type) {
                case GL_FLOAT:         return sizeof(float);        // 4 bytes
                case GL_UNSIGNED_INT:  return sizeof(unsigned int);// 4 bytes
                case GL_INT:           return sizeof(int);          // 4 bytes
                case GL_UNSIGNED_BYTE: return sizeof(unsigned char);// 1 byte
                case GL_BYTE:          return sizeof(signed char);  // 1 byte
                case GL_DOUBLE:        return sizeof(double);       // 8 bytes
                default:
                    throw std::runtime_error("Unknown GL type in sizeOfGLType");
            }
        }
    };

    class VertexBufferLayout {
    public:
        VertexBufferLayout() = default;
        ~VertexBufferLayout() = default;

        template<typename T>
        void push(unsigned int count);
        [[nodiscard]] inline unsigned int getStride() const { return m_stride; };
        inline const std::vector<VertexBufferElement> getElements() const { return m_elements; };

    private:
        std::vector<VertexBufferElement> m_elements;
        unsigned int m_stride = 0;
    };

} // Mineclone
