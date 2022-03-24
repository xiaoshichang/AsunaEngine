#pragma once
#include "../IndexBuffer.h"

namespace asuna
{
	class OpenglIndexBuffer : public IndexBuffer
	{
	protected:
		OpenglIndexBuffer() {}
	public:
		void Bind();
	public:
		unsigned int m_IndexBuffer = 0;

	public:
		static OpenglIndexBuffer* Create(IndexBufferCreateParam param);
	};
}

