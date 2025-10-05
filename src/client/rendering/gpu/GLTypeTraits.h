#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <type_traits>
#include "glad/glad.h"

namespace Mineclone {

    constexpr size_t getGlEnumSize(GLenum type) {
        switch (type) {
            case GL_BYTE:
            case GL_UNSIGNED_BYTE:
                return 1;
            case GL_SHORT:
            case GL_UNSIGNED_SHORT:
            case GL_HALF_FLOAT:
                return 2;
            case GL_INT:
            case GL_UNSIGNED_INT:
            case GL_FLOAT:
                return 4;
            case GL_DOUBLE:
                return 8;
            default:
                return 0;
        }
    }

    template<typename T>
    struct GLTypeTraits {
        static constexpr GLenum type = []() {
            if constexpr (std::is_same_v<T, float>) return GL_FLOAT;
            else if constexpr (std::is_same_v<T, int32_t>) return GL_INT;
            else if constexpr (std::is_same_v<T, uint32_t>) return GL_UNSIGNED_INT;
            else if constexpr (std::is_same_v<T, uint8_t>) return GL_UNSIGNED_BYTE;
            else if constexpr (std::is_same_v<T, int8_t>) return GL_BYTE;
            else if constexpr (std::is_same_v<T, int16_t>) return GL_SHORT;
            else if constexpr (std::is_same_v<T, uint16_t>) return GL_UNSIGNED_SHORT;
            else if constexpr (std::is_same_v<T, glm::vec2> ||
                               std::is_same_v<T, glm::vec3> ||
                               std::is_same_v<T, glm::vec4>) return GL_FLOAT;
            else static_assert(sizeof(T) == 0, "Unsupported vertex attribute type");
        }();

        static constexpr int count = []() {
            if constexpr (std::is_same_v<T, glm::vec2>) return 2;
            else if constexpr (std::is_same_v<T, glm::vec3>) return 3;
            else if constexpr (std::is_same_v<T, glm::vec4>) return 4;
            else return 1;
        }();

        static constexpr bool isInteger =
            std::is_integral_v<T> && !std::is_same_v<T, bool>;
    };

}
