#pragma once
#include <memory>
#include <d3d11.h>
#include "../Abstract/IndexBuffer.h"
namespace asuna
{
	class DirectX11IndexBuffer : public IndexBuffer
	{
	public:
		DirectX11IndexBuffer() = delete;
		DirectX11IndexBuffer(ID3D11Buffer* buffer, const std::shared_ptr<IndexBufferCreateParam>& param):
			m_IndexBuffer(buffer),
			m_ElementCount(param->m_ElementCount),
			m_Format(param->m_Format),
            m_StartIndex(param->m_StartIndex)
		{
		}

		virtual ~DirectX11IndexBuffer()
		{
			if (m_IndexBuffer != nullptr)
			{
				m_IndexBuffer->Release();
				m_IndexBuffer = nullptr;
			}
		}

		ID3D11Buffer* GetBuffer()
		{
			return m_IndexBuffer;
		}

		int GetElementCount()
		{
			return m_ElementCount;
		}

        int GetStartIndex()
        {
            return m_StartIndex;
        }

		DXGI_FORMAT GetDXGIFormat();

	private:
		ID3D11Buffer* m_IndexBuffer = nullptr;
		int m_ElementCount = 0;
        int m_StartIndex = 0;
		IndexBufferFormat m_Format;

	public:
		static std::shared_ptr<DirectX11IndexBuffer> Create(std::shared_ptr<IndexBufferCreateParam> param);
	};
}

