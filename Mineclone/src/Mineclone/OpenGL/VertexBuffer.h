#pragma once

#include <cstdint>

namespace Mineclone::OpenGL {
	class VertexBuffer {
	public:
		VertexBuffer(const void* data, uint32_t size);
		~VertexBuffer();

		void bind();
		void unbind();
	private:
		uint32_t  m_rendererId;
	};
}
