#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include "glad/glad.h"

#include "Debug/Log.h"

 namespace Mineclone::OpenGL {
	class Shader {
	public:
		Shader(const std::string& source);
		~Shader();

		void setUniformInt(const std::string& name, int value);
		void setUniformIntArray(const std::string& name, int* values, uint32_t count);

		void setUniformFloat(const std::string& name, float value);
		void setUniformFloat2(const std::string& name, const glm::vec2& value);
		void setUniformFloat3(const std::string& name, const glm::vec3& value);
		void setUniformFloat4(const std::string& name, const glm::vec4& value);

		void setUniformMat3(const std::string& name, const glm::mat3& value);
		void setUniformMat4(const std::string& name, const glm::mat4& value);
	private:
		// Utils
		std::string shaderTypeToString(GLenum type) {
			if(type == GL_VERTEX_SHADER) {
				return "Vertex Shader";
			} else if(type == GL_FRAGMENT_SHADER) {
				return "Fragment Shader";
			}
			m_logger.error("Type \"" + type + std::string("\" is not GL_VERTEX_SHADER or GL_FRAGMENT_SHADER"));
			return "Unkown";
		}

		GLenum shaderTypeFromString(const std::string& type) {
			if(type == "vertex") {
				return GL_VERTEX_SHADER;
			}
			else if(type == "fragment") {
				return GL_FRAGMENT_SHADER;
			}
			m_logger.error("Type \"" + type + std::string("\" is not vertex or fragment"));
			return 0;
		}

		std::unordered_map<GLenum, std::string> separateShaders(const std::string& source);
		std::string readShaderFile(const std::string& source);

		std::unordered_map<GLenum, unsigned int> compileShaders(const std::unordered_map<GLenum, std::string>& sources);
		void createProgram(unsigned int vertexShader, unsigned int fragmentShader);

		Log m_logger;
		uint32_t m_program;
	};
}
