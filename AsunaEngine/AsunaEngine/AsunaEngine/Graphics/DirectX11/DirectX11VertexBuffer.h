#pragma once
#include <memory>
#include <d3d11.h>
#include "../VertexBuffer.h"

namespace asuna
{
	class DirectX11VertexBuffer : public VertexBuffer
	{
	public:
		DirectX11VertexBuffer()
			: m_Buffer(nullptr)
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

	public:
		ID3D11Buffer* m_Buffer;

	public:
		static std::shared_ptr<DirectX11VertexBuffer> Create(std::shared_ptr<VertexBufferCreateParam> param);
	};

}

