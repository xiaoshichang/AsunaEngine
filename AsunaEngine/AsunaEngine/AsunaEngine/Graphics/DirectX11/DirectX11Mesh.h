#pragma once
#include <vector>
#include "../Mesh.h"
#include "DirectX11IndexBuffer.h"
#include "DirectX11VertexBuffer.h"

namespace asuna
{
	class DirectX11SubMesh : public SubMesh
	{
	protected:
		DirectX11SubMesh() {};

	public:
		static DirectX11SubMesh* Create(SubMeshCreateParam* param);
	};



	class DirectX11Mesh : public Mesh
	{
	protected:
		DirectX11Mesh() {}
	public:
		static DirectX11Mesh* Create(MeshCreateParam* param);
	};

}

