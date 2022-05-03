#pragma once
#include <memory>
#include "../Abstract/VertexBuffer.h"

namespace asuna
{

	class OpenglVertexBuffer : public VertexBuffer
	{
	public:
		OpenglVertexBuffer() = delete;

		OpenglVertexBuffer(unsigned int vbo, VertexBufferFormat format, int count, int stride, int offset):
			m_VBO(vbo),
			VertexBuffer(format, count, stride, offset)
		{
		}

		virtual ~OpenglVertexBuffer();
		void Bind(int index);

	private:
		unsigned int m_VBO = 0;
	public:
		static std::shared_ptr<OpenglVertexBuffer> Create(std::shared_ptr<VertexBufferCreateParam> param);

	};





	
}

