#pragma once
#include "../Foundation/Platform/Assert.h"
#include "RenderBuffer.h"

namespace asuna
{
	enum class VertexBufferFormat
	{
		Known = 0,
		F3,
		F4,
	};

	struct VertexBufferCreateParam
	{
		void* m_VertexData;
		int m_ElementCount;
		VertexBufferFormat m_Format;

		int GetFormatStride()
		{
			switch (m_Format)
			{
			case VertexBufferFormat::F3:			return sizeof(float) * 3;
			case VertexBufferFormat::F4:			return sizeof(float) * 4;
			default:
				ASUNA_ASSERT(false);
				return -1;
			}
		}
	};


	class VertexBuffer : public RenderBuffer
	{
	public:
		int m_ElementCount = 0;
		VertexBufferFormat m_Format = VertexBufferFormat::Known;
		int m_Stride = 0;
		int m_Offset = 0;

		
	};
}

