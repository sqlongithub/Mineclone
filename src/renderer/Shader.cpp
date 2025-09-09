#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "glad/glad.h"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Mineclone {

    Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
        // 1. Load shader source code from files
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile(vertexPath);
        std::ifstream fShaderFile(fragmentPath);

        if (!vShaderFile.is_open() || !fShaderFile.is_open()) {
            std::cerr << "ERROR: Cannot open shader file!" << std::endl;
            return;
        }

        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. Compile vertex shader
        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);

        // Check for compile errors
        GLint success;
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            std::cerr << "ERROR: Vertex shader compilation failed\n" << infoLog << std::endl;
        }

        // 3. Compile fragment shader
        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            std::cerr << "ERROR: Fragment shader compilation failed\n" << infoLog << std::endl;
        }

        // 4. Link shaders into a program
        m_id = glCreateProgram();
        glAttachShader(m_id, vertex);
        glAttachShader(m_id, fragment);
        glLinkProgram(m_id);

        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
            std::cerr << "ERROR: Shader program linking failed\n" << infoLog << std::endl;
        }

        // 5. Delete individual shaders after linking
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    Shader::~Shader() {
        glDeleteProgram(m_id);
        m_id = 0;
    }

    void Shader::bind() const {
        glUseProgram(m_id);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }

    void Shader::setUniform4f(const std::string &name, float x, float y, float z, float w) {
        glUniform4f(getUniformLocation(name), x, y, z, w);
    }

    void Shader::setUniformMat4(const std::string &name, const glm::mat4& matrix) const {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void Shader::compile() {

    }

    int Shader::getUniformLocation(const std::string &name) const {
        int location = glGetUniformLocation(m_id, name.c_str());
        if(location == -1)  {
            std::cout << "warn: uniform '" << name << "' doesn't exist";
        }
        return location;
    }
}