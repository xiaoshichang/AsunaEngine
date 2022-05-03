#pragma once
#include <memory>
#include "../Abstract/IndexBuffer.h"

namespace asuna
{
	class OpenglIndexBuffer : public IndexBuffer
	{
	public:
		OpenglIndexBuffer() = delete;
		OpenglIndexBuffer(unsigned int veo, const std::shared_ptr<IndexBufferCreateParam>& param):
			m_VEO(veo),
			m_ElementCount(param->m_ElementCount),
			m_Format(param->m_Format),
            m_StartIndex(param->m_StartIndex)
		{
		}

		~OpenglIndexBuffer() override;
		
		unsigned int GetVEO() const
		{
			return m_VEO;
		}

		int GetElementCount() const
		{
			return m_ElementCount;
		}

        int GetStartIndex() const
        {
            return m_StartIndex;
        }

		IndexBufferFormat GetFormat()
		{
			return m_Format;
		}


	public:
		unsigned short GetGLIndexType();
		void Bind() const;

	private:
		unsigned int m_VEO = 0;
		int m_ElementCount = 0;
        int m_StartIndex = 0;
		IndexBufferFormat m_Format;

	public:
		static std::shared_ptr<OpenglIndexBuffer> Create(const std::shared_ptr<IndexBufferCreateParam> &param);
	};
}

