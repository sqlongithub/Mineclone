#pragma once

#include "Log.h"

namespace Mineclone {
	class Shader {
	public:
		Shader(const std::string& vertexShader, const std::string& fragmentShader);
		~Shader();
	private:
		// Utils
		std::string shaderTypeToString(GLenum type) {
			if(type == GL_VERTEX_SHADER) {
				return "Vertex Shader";
			} else if(type == GL_FRAGMENT_SHADER) {
				return "Fragment Shader";
			}
			m_logger.error("Type \"" + type + std::string("\" is not GL_VERTEX_SHADER or GL_FRAGMENT_SHADER"));
		}

		std::string readShaderFile(const std::string& source);

		unsigned int compileShader(GLenum type, const std::string& source);
		void createProgram(unsigned int vertexShader, unsigned int fragmentShader);

		Log m_logger;
		uint32_t m_program;
	};
}