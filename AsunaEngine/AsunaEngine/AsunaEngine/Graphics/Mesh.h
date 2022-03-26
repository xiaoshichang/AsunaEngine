#pragma once
#include <memory>
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
		std::shared_ptr<VertexBufferCreateParam> m_PositionCreateParam;
		std::shared_ptr<VertexBufferCreateParam> m_NormalCreateParam;
		std::shared_ptr<VertexBufferCreateParam> m_TexcoordCreateParam;
		std::shared_ptr<IndexBufferCreateParam> m_IndexCreateParam;
		PrimitiveType m_PrimitiveType;

		SubMeshCreateParam() : 
			m_PositionCreateParam(nullptr),
			m_NormalCreateParam(nullptr),
			m_TexcoordCreateParam(nullptr),
			m_IndexCreateParam(nullptr),
			m_PrimitiveType(PrimitiveType::Unknown)
		{

		}
	};

	struct MeshCreateParam
	{
		int m_SubMeshCount;
		std::vector<std::shared_ptr<SubMeshCreateParam>> m_SubMeshCreateParam;

		MeshCreateParam() :
			m_SubMeshCount(0)
		{
		}
	};


	class SubMesh
	{
	public:
		std::shared_ptr<VertexBuffer> m_PositionBuffer = nullptr;
		std::shared_ptr<VertexBuffer> m_NormalBuffer = nullptr;
		std::shared_ptr<VertexBuffer> m_TexCoordBuffer = nullptr;
		std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
		PrimitiveType m_PrimitiveType = PrimitiveType::Unknown;
	};

	class Mesh
	{
	public:
		std::vector<std::shared_ptr<SubMesh>> m_SubMeshes;
	};




}


