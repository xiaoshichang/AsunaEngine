#pragma once
#include "../Mesh.h"
#include "DirectX11IndexBuffer.h"
#include "DirectX11VertexBuffer.h"

namespace asuna
{
	class DirectX11Mesh : public Mesh
	{
	protected:
		DirectX11Mesh() {}
	public:
		DirectX11VertexBuffer* m_VertexBuffer = nullptr;;
		DirectX11IndexBuffer* m_IndexBuffer = nullptr;
	public:
		static DirectX11Mesh* Create(MeshCreateParam param);
	};

}

