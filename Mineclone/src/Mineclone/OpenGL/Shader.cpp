#include <unordered_map>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Debug/Log.h"

namespace Mineclone::OpenGL {
	Shader::Shader(const std::string& source)
		: m_logger("Shader") {
		std::unordered_map<GLenum, std::string> sources = separateShaders(source);
		std::unordered_map<GLenum, unsigned int> shaders = compileShaders(sources);
		createProgram(shaders[GL_VERTEX_SHADER], shaders[GL_FRAGMENT_SHADER]);
		glUseProgram(m_rendererId);
	}

	Shader::~Shader() {
		glDeleteProgram(m_rendererId);
	}

	std::unordered_map<GLenum, std::string> Shader::separateShaders(const std::string& shaderFile) {
		// Courtesy of Hazel's Shader class
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";

		const std::string& source = readShaderFile(shaderFile);

		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line

		while(pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			if(eol == std::string::npos) {
				m_logger.error("Could not separate shaders: Syntax error in shader");
			}
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			if(!shaderTypeFromString(type)) {
				m_logger.error("Could not separate shaders: Invalid shader type specified");
			}

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			if(nextLinePos == std::string::npos) {
				m_logger.error("Could not separate shaders: Syntax error");
			}
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[shaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	std::string Shader::readShaderFile(const std::string& source) {
		std::ifstream file(source, std::ios::in | std::ios::binary);
		if(!file.is_open()) {
			m_logger.error("Could not open shader at \"" + source + "\"");
		}
		std::stringstream buffer;
		buffer << file.rdbuf();

		return buffer.str();
	}

	std::unordered_map<GLenum, unsigned int> Shader::compileShaders(const std::unordered_map<GLenum, std::string>& sources) {
		std::unordered_map<GLenum, unsigned int> output;

		for(auto&& [type, source] : sources) {
			unsigned int shader = glCreateShader(type);
			const char* src = source.c_str();
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

			}
			output[type] = shader;
		}

		return output;
	}

	void Shader::createProgram(unsigned int vertexShader, unsigned int fragmentShader) {
		unsigned int programId = glCreateProgram();

		glAttachShader(programId, vertexShader);
		glAttachShader(programId, fragmentShader);
		glLinkProgram(programId);
		glValidateProgram(programId);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		m_rendererId = programId;
	}

	void Shader::setUniformInt(const std::string& name, int value) {
		GLint location = glGetUniformLocation(m_rendererId, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::setUniformIntArray(const std::string& name, int* values, uint32_t count) {
		GLint location = glGetUniformLocation(m_rendererId, name.c_str());
		glUniform1iv(location, count, values);
	}

	void Shader::setUniformFloat(const std::string& name, float value) {
		GLint location = glGetUniformLocation(m_rendererId, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::setUniformFloat2(const std::string& name, const glm::vec2& value) {
		GLint location = glGetUniformLocation(m_rendererId, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void Shader::setUniformFloat3(const std::string& name, const glm::vec3& value) {
		GLint location = glGetUniformLocation(m_rendererId, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::setUniformFloat4(const std::string& name, const glm::vec4& value) {
		GLint location = glGetUniformLocation(m_rendererId, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::setUniformMat3(const std::string& name, const glm::mat3& value) {
		GLint location = glGetUniformLocation(m_rendererId, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::setUniformMat4(const std::string& name, const glm::mat4& value) {
		GLint location = glGetUniformLocation(m_rendererId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

}
