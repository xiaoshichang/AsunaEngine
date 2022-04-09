#pragma once
#include <memory>
#include <vector>
#include "../Abstract/Mesh.h"
#include "OpenglVertexBuffer.h"
#include "OpenglIndexBuffer.h"

namespace asuna
{
	class OpenglSubMesh : public SubMesh
	{
	public:
		OpenglSubMesh() = delete;
		OpenglSubMesh(
			unsigned int vao,
			std::shared_ptr<VertexBuffer> position,
			std::shared_ptr<VertexBuffer> normal,
			std::shared_ptr<VertexBuffer> texcoord,
			std::shared_ptr<IndexBuffer> index,
			PrimitiveType pt,
            int materialIndex) :
			m_VAO(vao),
			SubMesh(position, normal, texcoord, index, pt, materialIndex)
		{
		}

        ~OpenglSubMesh();

	public:
		unsigned int GetGLPrimitive();
		unsigned int m_VAO;

	public:
		static std::shared_ptr<OpenglSubMesh> Create(const std::shared_ptr<SubMeshCreateParam>& param);
	};



	class OpenglMesh : public Mesh
	{
	public:
		OpenglMesh() {}
	public:
		
	public:
		static std::shared_ptr<OpenglMesh> Create(const std::shared_ptr<MeshCreateParam>& param);
	};
}
