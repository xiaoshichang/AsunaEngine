#pragma once
#include <memory>
#include <vector>
#include "../Abstract/Mesh.h"
#include "OpenglVertexBuffer.h"
#include "OpenglIndexBuffer.h"

namespace asuna
{
	class OpengSubMesh : public SubMesh
	{
	public:
		OpengSubMesh() = delete;
		OpengSubMesh(
			unsigned int vao,
			std::shared_ptr<VertexBuffer> position,
			std::shared_ptr<VertexBuffer> normal,
			std::shared_ptr<VertexBuffer> texcoord,
			std::shared_ptr<IndexBuffer> index,
			PrimitiveType pt) :
			m_VAO(vao),
			SubMesh(position, normal, texcoord, index, pt)
		{
		}

		virtual ~OpengSubMesh();

	public:
		unsigned int GetGLPrimitive();
		unsigned int m_VAO;

	public:
		static std::shared_ptr<OpengSubMesh> Create(std::shared_ptr<SubMeshCreateParam> param);
	};



	class OpenglMesh : public Mesh
	{
	public:
		OpenglMesh() {}
	public:
		
	public:
		static std::shared_ptr<OpenglMesh> Create(std::shared_ptr<MeshCreateParam> param);
	};
}
