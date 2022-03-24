#pragma once
#include "../VertexBuffer.h"

namespace asuna
{

	class OpenglVertexBuffer : public VertexBuffer
	{
	protected:
		OpenglVertexBuffer() {}
	public:
		void Bind();
	public:
		unsigned int m_VertexBuffer = 0;
	public:
		static OpenglVertexBuffer* Create(VertexBufferCreateParam param);

	};





	
}

