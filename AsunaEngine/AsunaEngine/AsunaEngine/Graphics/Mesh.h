#pragma once
#include <vector>
#include "IndexBuffer.h"
#include "VertexBuffer.h"


namespace asuna
{
	enum class PrimitiveType
	{
		Unknown = 0,
		Point,
		Line,
		Triangle,
	};


	struct SubMeshCreateParam
	{
		VertexBufferCreateParam* m_PositionCreateParam;
		VertexBufferCreateParam* m_NormalCreateParam;
		VertexBufferCreateParam* m_TexcoordCreateParam;
		IndexBufferCreateParam* m_IndexCreateParam;
		PrimitiveType m_PrimitiveType;

		SubMeshCreateParam() : 
			m_PositionCreateParam(nullptr),
			m_NormalCreateParam(nullptr),
			m_TexcoordCreateParam(nullptr),
			m_IndexCreateParam(nullptr),
			m_PrimitiveType(PrimitiveType::Unknown)
		{

		}
		~SubMeshCreateParam()
		{
			if (m_PositionCreateParam != nullptr)
			{
				delete m_PositionCreateParam;
			}
			if (m_NormalCreateParam != nullptr)
			{
				delete m_NormalCreateParam;
			}
			if (m_TexcoordCreateParam != nullptr)
			{
				delete m_TexcoordCreateParam;
			}
			if (m_IndexCreateParam != nullptr)
			{
				delete m_IndexCreateParam;
			}
		}
	};

	struct MeshCreateParam
	{
		int m_SubMeshCount;
		SubMeshCreateParam* m_SubMeshCreateParam;

		MeshCreateParam() :
			m_SubMeshCount(0),
			m_SubMeshCreateParam(nullptr)
		{
		}

		~MeshCreateParam()
		{
			if (m_SubMeshCreateParam != nullptr)
			{
				delete[] m_SubMeshCreateParam;
			}
		}
	};


	class SubMesh
	{
	public:
		VertexBuffer* m_PositionBuffer = nullptr;
		VertexBuffer* m_NormalBuffer = nullptr;
		VertexBuffer* m_TexCoordBuffer = nullptr;
		IndexBuffer* m_IndexBuffer = nullptr;
		PrimitiveType m_PrimitiveType = PrimitiveType::Unknown;
	};

	class Mesh
	{
	public:
		std::vector<SubMesh*> m_SubMeshes;
	};




}


