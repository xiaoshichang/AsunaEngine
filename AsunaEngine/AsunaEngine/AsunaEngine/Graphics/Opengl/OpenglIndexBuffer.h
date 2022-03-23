#pragma once
#include "../IndexBuffer.h"
namespace asuna
{
	class OpenglIndexBuffer : public IndexBuffer
	{
	public:
		int m_IndexCount;
		unsigned int m_IndexBuffer;
	};
}

