#pragma once
#include <memory>
#include "../IndexBuffer.h"

namespace asuna
{
	class OpenglIndexBuffer : public IndexBuffer
	{
	public:
		OpenglIndexBuffer() {}
	public:
		unsigned short GetGLIndexType();
		void Bind();
	public:
		unsigned int m_IndexBuffer = 0;

	public:
		static std::shared_ptr<OpenglIndexBuffer> Create(std::shared_ptr<IndexBufferCreateParam> param);
	};
}

