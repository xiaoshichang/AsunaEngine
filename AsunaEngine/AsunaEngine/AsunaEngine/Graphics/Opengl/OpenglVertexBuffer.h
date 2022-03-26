#pragma once
#include <memory>
#include "../VertexBuffer.h"

namespace asuna
{

	class OpenglVertexBuffer : public VertexBuffer
	{
	public:
		OpenglVertexBuffer() {}
	public:
		void Bind(int index);
	public:
		unsigned int m_VertexBuffer = 0;
	public:
		static std::shared_ptr<OpenglVertexBuffer> Create(std::shared_ptr<VertexBufferCreateParam> param);

	};





	
}

