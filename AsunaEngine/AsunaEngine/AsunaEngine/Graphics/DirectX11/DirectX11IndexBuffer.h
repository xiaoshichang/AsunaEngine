#pragma once
#include <d3d11.h>
#include "../IndexBuffer.h"
namespace asuna
{
	class DirectX11IndexBuffer : public IndexBuffer
	{
	protected:
		DirectX11IndexBuffer(){}
	public:
		ID3D11Buffer* mIndexBuffer = nullptr;
	public:
		static DirectX11IndexBuffer* Create(void* data, size_t elementCount);
	};
}

