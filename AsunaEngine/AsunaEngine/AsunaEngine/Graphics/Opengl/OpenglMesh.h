#pragma once
#include "../Mesh.h"
#include "OpenglVertexBuffer.h"
#include "OpenglIndexBuffer.h"

namespace asuna
{
	class OpenglMesh : public Mesh
	{
	protected:
		OpenglMesh() {}
	public:
		unsigned int m_VertexArray = 0;

	public:
		static OpenglMesh* Create(MeshCreateParam param);
	};
}
