#pragma once
#include <vector>
#include <memory>
#include "../Abstract/Mesh.h"
#include "DirectX11IndexBuffer.h"
#include "DirectX11VertexBuffer.h"
#include "../../Foundation/Logger/Logger.h"

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
			PrimitiveType pt,
            int materialIndex):
			SubMesh(position, normal, texcoord, index, pt, materialIndex)
		{
		}

		~DirectX11SubMesh() override = default;

	public:
		static std::shared_ptr<DirectX11SubMesh> Create(const std::shared_ptr<SubMeshCreateParam>& param);
	};



	class DirectX11Mesh : public Mesh
	{
	public:
		DirectX11Mesh() = default;
		~DirectX11Mesh() override = default;

	public:
		static std::shared_ptr<DirectX11Mesh> Create(std::shared_ptr<MeshCreateParam> param);
	};

}

