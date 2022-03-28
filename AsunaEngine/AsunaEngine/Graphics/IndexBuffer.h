#pragma once
#include "../Foundation/Platform/Assert.h"
#include "RenderBuffer.h"

namespace asuna
{
	enum class IndexBufferFormat {
		UINT16,
		UINT32,
	};

	struct IndexBufferCreateParam
	{
		void* m_IndexData;
		int m_ElementCount;
		IndexBufferFormat m_Format;

		int GetFormatSize()
		{
			switch (m_Format)
			{
				case IndexBufferFormat::UINT16:			return 2;
				case IndexBufferFormat::UINT32:			return 4;
				default:
					ASUNA_ASSERT(false);
					return -1;
			}
		}
	};

	class IndexBuffer : public RenderBuffer
	{
	public:
		virtual ~IndexBuffer()
		{
		}
		
	};
}


