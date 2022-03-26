#pragma once
#include <memory>
#include <vector>
#include "../Mesh.h"
#include "OpenglVertexBuffer.h"
#include "OpenglIndexBuffer.h"

namespace asuna
{
	class OpengSubMesh : public SubMesh
	{
	public:
		OpengSubMesh() 
		{
		}

		virtual ~OpengSubMesh()
		{
		}

	public:
		unsigned int GetGLPrimitive();

		unsigned int m_VertexArray = 0;

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
