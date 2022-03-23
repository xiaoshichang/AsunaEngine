#pragma once
#include <d3d11.h>
#include "../VertexBuffer.h"

namespace asuna
{
	class DirectX11VertexBuffer : public VertexBuffer
	{
	public:

		ID3D11Buffer* m_Buffer;

	public:
		static DirectX11VertexBuffer* Create(void* data, int sizeInBytes);
	};

}

