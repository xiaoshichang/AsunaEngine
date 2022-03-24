#pragma once
#include <d3d11.h>
#include "../VertexBuffer.h"

namespace asuna
{
	class DirectX11VertexBuffer : public VertexBuffer
	{
	public:

		ID3D11Buffer* m_Buffer;
		int m_Stride = 0;
		int m_Offset = 0;

	public:
		static DirectX11VertexBuffer* Create(VertexBufferCreateParam param);
	};

}

