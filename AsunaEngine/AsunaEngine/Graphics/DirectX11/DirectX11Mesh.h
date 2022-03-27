#pragma once
#include <vector>
#include "../Mesh.h"
#include "DirectX11IndexBuffer.h"
#include "DirectX11VertexBuffer.h"

namespace asuna
{
	class DirectX11SubMesh : public SubMesh
	{
	public:
		DirectX11SubMesh() 
		{
		}

		virtual ~DirectX11SubMesh()
		{
		}

	public:
		static std::shared_ptr<DirectX11SubMesh> Create(std::shared_ptr<SubMeshCreateParam> param);
	};



	class DirectX11Mesh : public Mesh
	{
	public:
		DirectX11Mesh() 
		{
		}

		virtual ~DirectX11Mesh()
		{
		}

	public:
		static std::shared_ptr<DirectX11Mesh> Create(std::shared_ptr<MeshCreateParam> param);
	};

}

