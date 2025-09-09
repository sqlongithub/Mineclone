#pragma once

#include "glm/fwd.hpp"

namespace Mineclone {
    class Shader {
    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        ~Shader();

        void bind() const;
        void unbind() const;

        void setUniform4f(const std::string& name, float x, float y, float z, float w);
        void setUniformMat4(const std::string &name, const glm::mat4& matrix) const;
    private:
        void compile();
        [[nodiscard]] int getUniformLocation(const std::string& name) const;
        unsigned int m_id;

    };
}
