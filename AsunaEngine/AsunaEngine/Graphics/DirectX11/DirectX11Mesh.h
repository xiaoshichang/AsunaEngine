#pragma once
#include <vector>
#include <memory>
#include "../Mesh.h"
#include "DirectX11IndexBuffer.h"
#include "DirectX11VertexBuffer.h"

namespace asuna
{
	class DirectX11SubMesh : public SubMesh
	{
	public:
		DirectX11SubMesh() = delete;
		DirectX11SubMesh(
			std::shared_ptr<VertexBuffer> position,
			std::shared_ptr<VertexBuffer> normal,
			std::shared_ptr<VertexBuffer> texcoord,
			std::shared_ptr<IndexBuffer> index,
			PrimitiveType pt) :
			SubMesh(position, normal, texcoord, index, pt)
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

