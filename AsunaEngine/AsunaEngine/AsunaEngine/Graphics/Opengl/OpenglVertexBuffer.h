#pragma once
#include "../VertexBuffer.h"

namespace asuna
{

	class OpenglVertexBuffer : public VertexBuffer
	{
	protected:
		OpenglVertexBuffer() {}
	public:
		void Bind(int index);
	public:
		unsigned int m_VertexBuffer = 0;
	public:
		static OpenglVertexBuffer* Create(VertexBufferCreateParam* param);

	};





	
}

