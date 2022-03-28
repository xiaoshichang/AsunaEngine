#pragma once
#include <memory>
#include "../IndexBuffer.h"

namespace asuna
{
	class OpenglIndexBuffer : public IndexBuffer
	{
	public:
		OpenglIndexBuffer() = delete;
		OpenglIndexBuffer(unsigned int veo, int count, IndexBufferFormat format):
			m_VEO(veo),
			m_ElementCount(count),
			m_Format(format)
		{
		}

		virtual ~OpenglIndexBuffer();
		
		unsigned int GetVEO()
		{
			return m_VEO;
		}

		int GetElementCount()
		{
			return m_ElementCount;
		}

		IndexBufferFormat GetFormat()
		{
			return m_Format;
		}


	public:
		unsigned short GetGLIndexType();
		void Bind();

	private:
		unsigned int m_VEO = 0;
		int m_ElementCount = 0;
		IndexBufferFormat m_Format;

	public:
		static std::shared_ptr<OpenglIndexBuffer> Create(std::shared_ptr<IndexBufferCreateParam> param);
	};
}

