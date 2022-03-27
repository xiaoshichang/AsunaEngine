#pragma once
#include <memory>
#include <d3d11.h>
#include "../IndexBuffer.h"
namespace asuna
{
	class DirectX11IndexBuffer : public IndexBuffer
	{
	public:
		DirectX11IndexBuffer()
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


	public:
		DXGI_FORMAT GetDXGIFormat();
	public:
		ID3D11Buffer* m_IndexBuffer = nullptr;
	public:
		static std::shared_ptr<DirectX11IndexBuffer> Create(std::shared_ptr<IndexBufferCreateParam> param);
	};
}

