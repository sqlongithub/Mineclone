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

        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        printShaderCompileLog(vertex, "Vertex");

        GLint success;
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            std::cerr << "ERROR: Vertex shader compilation failed\n" << infoLog << std::endl;
        }

        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        printShaderCompileLog(fragment, "Fragment");

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            std::cerr << "ERROR: Fragment shader compilation failed\n" << infoLog << std::endl;
        }

        m_id = glCreateProgram();
        glAttachShader(m_id, vertex);
        glAttachShader(m_id, fragment);
        glLinkProgram(m_id);
        printProgramLinkLog(m_id);

        GLint nUniforms;
        glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &nUniforms);
        std::cout << "Active uniforms: " << nUniforms << std::endl;
        for(int i = 0; i < nUniforms; ++i) {
            char name[256];
            GLsizei length;
            GLint size;
            GLenum type;
            glGetActiveUniform(m_id, i, 256, &length, &size, &type, name);
            std::cout << "Uniform[" << i << "] = " << name << std::endl;
        }

        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
            std::cerr << "ERROR: Shader program linking failed\n" << infoLog << std::endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);

#ifdef DEBUG
        std::cout << "Shader program ID: " << m_id << std::endl;
#endif
    }

    Shader::~Shader() {
        std::cout << "Shader deleted :((\n";
        glDeleteProgram(m_id);
        m_id = 0;
    }

    void Shader::printShaderCompileLog(GLuint shader, const std::string& type) {
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            std::string infoLog(length, ' ');
            glGetShaderInfoLog(shader, length, nullptr, infoLog.data());
            std::cerr << "ERROR: " << type << " shader compilation failed\n" << infoLog << std::endl;
        }
    }

    void Shader::printProgramLinkLog(GLuint program) {
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            GLint length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
            std::string infoLog(length, ' ');
            glGetProgramInfoLog(program, length, nullptr, infoLog.data());
            std::cerr << "ERROR: Shader program linking failed\n" << infoLog << std::endl;
        }
    }

    void Shader::bind() const {
        glUseProgram(m_id);
    }

    void Shader::unbind() const {
        glUseProgram(0);
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