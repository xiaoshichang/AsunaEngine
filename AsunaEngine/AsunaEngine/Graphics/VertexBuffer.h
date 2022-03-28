#pragma once
#include "../Foundation/Platform/Assert.h"
#include <memory>
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
		VertexBuffer() = delete;
		VertexBuffer(VertexBufferFormat format, int count, int stride, int offset) :
			m_Format(format),
			m_ElementCount(count),
			m_Stride(stride),
			m_Offset(offset)
		{
		}

		VertexBufferFormat GetFormat()
		{
			return m_Format;
		}

		int GetElementCount()
		{
			return m_ElementCount;
		}

		int GetStride()
		{
			return m_Stride;
		}

		int GetOffset()
		{
			return m_Offset;
		}

		virtual ~VertexBuffer()
		{
		}

	private:
		VertexBufferFormat m_Format;
		int m_ElementCount = 0;
		int m_Stride = 0;
		int m_Offset = 0;

		
	};
}

