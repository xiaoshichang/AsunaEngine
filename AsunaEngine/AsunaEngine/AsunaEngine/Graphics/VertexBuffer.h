#pragma once
#include "../Foundation/Platform/Assert.h"
#include "RenderBuffer.h"

namespace asuna
{
	enum class VertexBufferFormat
	{
		// P - position
		// C - color
		// F - float
		P3F,				
		P4F,
		P3F_C3F,
		P3F_C4F
	};

	struct VertexBufferCreateParam
	{
		void* m_VertexData;
		int m_ElementCount;
		VertexBufferFormat m_Format;

		int GetFormatSize()
		{
			switch (m_Format)
			{
			case VertexBufferFormat::P3F:			return sizeof(float) * 3;
			case VertexBufferFormat::P4F:			return sizeof(float) * 4;
			case VertexBufferFormat::P3F_C3F:		return sizeof(float) * 6;
			case VertexBufferFormat::P3F_C4F:		return sizeof(float) * 7;
			default:
				ASUNA_ASSERT(false);
			}
		}
	};


	class VertexBuffer : public RenderBuffer
	{
		
	};
}

