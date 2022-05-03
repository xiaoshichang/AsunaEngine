#pragma once
#include <memory>
#include <d3d11.h>
#include "../Abstract/VertexBuffer.h"

namespace asuna
{
	class DirectX11VertexBuffer : public VertexBuffer
	{
	public:
		DirectX11VertexBuffer() = delete;
		DirectX11VertexBuffer(ID3D11Buffer* buffer, VertexBufferFormat format, int count, int stride, int offset) :
			m_Buffer(buffer),
			VertexBuffer(format, count, stride, offset)
		{
		}

		virtual ~DirectX11VertexBuffer()
		{
			if (m_Buffer != nullptr)
			{
				m_Buffer->Release();
				m_Buffer = nullptr;
			}
		}

		ID3D11Buffer* GetBuffer()
		{
			return m_Buffer;
		}

	private:
		ID3D11Buffer* m_Buffer;

	public:
		static std::shared_ptr<DirectX11VertexBuffer> Create(std::shared_ptr<VertexBufferCreateParam> param);
	};

}

