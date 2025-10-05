#pragma once
#include <glm/glm.hpp>
#include <glaze/glaze.hpp>

template <>
struct glz::meta<glm::vec3>
{
    using T = glm::vec3;
    static constexpr auto value = array(&T::x, &T::y, &T::z);
};