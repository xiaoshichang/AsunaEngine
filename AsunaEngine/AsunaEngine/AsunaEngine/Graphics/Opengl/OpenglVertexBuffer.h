#pragma once
#include <memory>
#include "../VertexBuffer.h"

namespace asuna
{

	class OpenglVertexBuffer : public VertexBuffer
	{
	public:
		OpenglVertexBuffer() 
		{
		}

		virtual ~OpenglVertexBuffer();

	public:
		void Bind(int index);
	public:
		unsigned int m_VBO = 0;
	public:
		static std::shared_ptr<OpenglVertexBuffer> Create(std::shared_ptr<VertexBufferCreateParam> param);

	};





	
}

