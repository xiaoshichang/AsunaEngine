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
		DXGI_FORMAT GetDXGIFormat();
	public:
		ID3D11Buffer* m_IndexBuffer = nullptr;
	public:
		static DirectX11IndexBuffer* Create(IndexBufferCreateParam* param);
	};
}

