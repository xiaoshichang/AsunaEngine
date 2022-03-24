#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"


namespace asuna
{

	struct MeshCreateParam
	{
		VertexBufferCreateParam m_VertexBufferCreateParam;
		IndexBufferCreateParam m_IndexBufferCreateParam;
	};



	class Mesh
	{

	public:
		IndexBuffer* m_IndexBuffer;
		VertexBuffer* m_VertexBuffer;
	};




}


