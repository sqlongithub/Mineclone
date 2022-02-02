#include <cstdint>

#include <glad/glad.h>

#include "VertexBuffer.h"

namespace Mineclone::OpenGL {

	VertexBuffer::VertexBuffer(const void* data, uint32_t size) {
		glGenBuffers(1, &m_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_rendererId);
	}

	void VertexBuffer::bind() {
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
	}

	void VertexBuffer::unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
