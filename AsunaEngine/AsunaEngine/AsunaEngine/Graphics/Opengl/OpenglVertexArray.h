#pragma once

#include "OpenglVertexBuffer.h"
#include "OpenglIndexBuffer.h"

namespace asuna
{
	class OpenglVertexArray
	{
	protected:
		OpenglVertexArray()
		{}

	public:
		OpenglVertexBuffer* m_VertexBuffer = nullptr;
		OpenglIndexBuffer* m_IndexBuffer = nullptr;
		unsigned int m_VertexArray = 0;

	public:
		static OpenglVertexArray* Create(void* vertexBuffer, int vertexCount, void* indexBuffer, int indexCount);
	};
}
