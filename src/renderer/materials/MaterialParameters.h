#pragma once

#include <variant>
#include <vector>
#include <glm/vec3.hpp>
#include "glm/vec4.hpp"
#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"
#include <glm/mat4x4.hpp>
#include <string>
#include <iostream>

namespace Mineclone {

    using MaterialParamValue = std::variant<
            float,
            int,
            glm::vec3,
            glm::vec4,
            glm::mat4,
            std::vector<float>,
            std::vector<glm::vec3>
    >;

    class MaterialParameters {
    public:
        void setFloat(const std::string& name, float value) {
            m_parameters[name] = value;
        };
        void setInt(const std::string& name, int value) {
            m_parameters[name] = value;
        };
        void setVec3(const std::string& name, const glm::vec3& value) {
            m_parameters[name] = value;
        };
        void setVec4(const std::string& name, const glm::vec4& value) {
            m_parameters[name] = value;
        };
        void setMat4(const std::string& name, const glm::mat4& value) {
            m_parameters[name] = value;
        };
        void setFloatArray(const std::string& name, const std::vector<float>& values) {
            m_parameters[name] = values;
        };
        void setVec3Array(const std::string& name, const std::vector<glm::vec3>& values) {
            m_parameters[name] = values;
        };

        void applyToShader(const Shader& shader) const {
            for (const auto& [name, value] : m_parameters) {
                GLint location = glGetUniformLocation(shader.getID(), name.c_str());
                if (location == -1) continue; // skip missing uniforms

                std::visit([location](auto&& val) {
                    using T = std::decay_t<decltype(val)>;
                    if constexpr (std::is_same_v<T, float>) {
                        glUniform1f(location, val);
                    } else if constexpr (std::is_same_v<T, int>) {
                        glUniform1i(location, val);
                    } else if constexpr (std::is_same_v<T, glm::vec3>) {
                        glUniform3f(location, val.x, val.y, val.z);
                    } else if constexpr (std::is_same_v<T, glm::vec4>) {
                        glUniform4f(location, val.x, val.y, val.z, val.w);
                    } else if constexpr (std::is_same_v<T, glm::mat4>) {
                        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val));
                    } else if constexpr (std::is_same_v<T, std::vector<float>>) {
                        if (!val.empty()) glUniform1fv(location, static_cast<GLsizei>(val.size()), val.data());
                    } else if constexpr (std::is_same_v<T, std::vector<glm::vec3>>) {
            //            std::cout << "passing vec\n";
                        if (!val.empty()) {
                            std::vector<float> flattened;
                            flattened.reserve(val.size() * 3);
                            for (auto& v : val) {
                                flattened.push_back(v.x);
                                flattened.push_back(v.y);
                                flattened.push_back(v.z);
                            }
                            glUniform3fv(location, static_cast<GLsizei>(val.size()), flattened.data());
                        }
                    }
                }, value);
            }
        }

    private:
        std::unordered_map <std::string, MaterialParamValue> m_parameters;
    };
}