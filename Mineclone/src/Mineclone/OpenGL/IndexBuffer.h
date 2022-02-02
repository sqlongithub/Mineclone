#pragma once

#include <cstdint>

namespace Mineclone::OpenGL {
	class IndexBuffer {
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		void bind();
		void unbind();
	private:
		uint32_t m_rendererId;
		uint32_t m_count;
	};
}
