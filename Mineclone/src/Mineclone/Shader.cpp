#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

namespace Mineclone {
	Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
		: m_logger("Shader") {
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexShader;
		sources[GL_FRAGMENT_SHADER] = fragmentShader;

		unsigned int vs = compileShader(GL_VERTEX_SHADER, sources[GL_VERTEX_SHADER]);
		unsigned int fs = compileShader(GL_FRAGMENT_SHADER, sources[GL_FRAGMENT_SHADER]);
		createProgram(vs, fs);
		glUseProgram(m_program);
	}

	Shader::~Shader() {
		glDeleteProgram(m_program);
	}

	std::string Shader::readShaderFile(const std::string& source) {
		std::ifstream file(source);
		std::stringstream buffer;
		buffer << file.rdbuf();

		return buffer.str();
	}

	unsigned int Shader::compileShader(GLenum type, const std::string& source) {
		unsigned int shader = glCreateShader(type);
		const std::string& content = readShaderFile(source);
		const char* src = content.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		int result;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if(result == GL_FALSE) {
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(shader, length, &length, message);

			m_logger.error("Failed to compile shader: " + shaderTypeToString(type) + " at \"" + source + "\"");
			m_logger.error(message);

			glDeleteShader(shader);
			return 0;
		}

		return shader;
	}

	void Shader::createProgram(unsigned int vertexShader, unsigned int fragmentShader) {
		unsigned int program = glCreateProgram();

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		m_program = program;
	}
}
