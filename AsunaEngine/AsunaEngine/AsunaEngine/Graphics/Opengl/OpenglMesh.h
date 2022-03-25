#pragma once
#include <vector>
#include "../Mesh.h"
#include "OpenglVertexBuffer.h"
#include "OpenglIndexBuffer.h"

namespace asuna
{
	class OpengSubMesh : public SubMesh
	{
	protected:
		OpengSubMesh() {}

	public:
		unsigned int GetGLPrimitive();

		unsigned int m_VertexArray = 0;

	public:
		static OpengSubMesh* Create(SubMeshCreateParam* param);
	};



	class OpenglMesh : public Mesh
	{
	protected:
		OpenglMesh() {}
	public:
		
	public:
		static OpenglMesh* Create(MeshCreateParam* param);
	};
}
