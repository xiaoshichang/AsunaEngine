#pragma once
#include <memory>
#include <d3d11.h>
#include "../VertexBuffer.h"

namespace asuna
{
	class DirectX11VertexBuffer : public VertexBuffer
	{
	public:
		ID3D11Buffer* m_Buffer;

	public:
		static std::shared_ptr<DirectX11VertexBuffer> Create(std::shared_ptr<VertexBufferCreateParam> param);
	};

}

