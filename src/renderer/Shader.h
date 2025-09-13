#pragma once

#include "glm/fwd.hpp"
#include "glad/glad.h"

namespace Mineclone {
    class Shader {
    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        ~Shader();
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void bind() const;
        void unbind() const;

        void setUniform4f(const std::string& name, float x, float y, float z, float w);
        void setUniformMat4(const std::string &name, const glm::mat4& matrix, bool transpose = false) const;

        [[nodiscard]] unsigned int getID() const { return m_id; }
    private:
        void compile();
        [[nodiscard]] int getUniformLocation(const std::string& name) const;
        unsigned int m_id;

        void printShaderCompileLog(unsigned int shader, const std::string &type);

        void printProgramLinkLog(unsigned int program);
    };
}
