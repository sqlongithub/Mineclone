#pragma once

#include "glm/fwd.hpp"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace Mineclone {
    class Shader {
    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        ~Shader();
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void bind() const;
        void unbind() const;

        inline void setUniform1f(const std::string& name, float v0) {
            glUniform1f(getUniformLocation(name), v0);
        }

        inline void setUniform2f(const std::string& name, const glm::vec2& v) {
            glUniform2f(getUniformLocation(name), v.x, v.y);
        }

        inline void setUniform3f(const std::string& name, const glm::vec3& v) {
            glUniform3f(getUniformLocation(name), v.x, v.y, v.z);
        }

        inline void setUniform4f(const std::string& name, const glm::vec4& v) {
            glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w);
        }

        inline void setUniformMat3(const std::string& name, const glm::mat3& mat, bool transpose = false) const {
            glUniformMatrix3fv(getUniformLocation(name), 1, transpose, glm::value_ptr(mat));
        }

        inline void setUniformMat4(const std::string& name, const glm::mat4& mat, bool transpose = false) const {
            glUniformMatrix4fv(getUniformLocation(name), 1, transpose, glm::value_ptr(mat));
        }

        inline void setUniformInt(const std::string& name, int value) const {
            glUniform1i(getUniformLocation(name), value);
        }

        inline void setUniformIntArray(const std::string& name, const int* values, int count) const {
            glUniform1iv(getUniformLocation(name), count, values);
        }

        inline void setUniformFloatArray(const std::string& name, const float* values, int count) const {
            glUniform1fv(getUniformLocation(name), count, values);
        }

        [[nodiscard]] unsigned int getID() const { return m_id; }
    private:
        void compile();
        [[nodiscard]] int getUniformLocation(const std::string& name) const;
        unsigned int m_id;

        void printShaderCompileLog(unsigned int shader, const std::string &type);

        void printProgramLinkLog(unsigned int program);
    };
}
