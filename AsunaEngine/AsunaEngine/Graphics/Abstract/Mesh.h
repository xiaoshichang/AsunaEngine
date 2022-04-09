#pragma once
#include <memory>
#include <vector>
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include <assimp/scene.h>
#include "../../Foundation/Logger/Logger.h"


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
        int m_MaterialIndex;

		SubMeshCreateParam() : 
			m_PositionCreateParam(nullptr),
			m_NormalCreateParam(nullptr),
			m_TexcoordCreateParam(nullptr),
			m_IndexCreateParam(nullptr),
			m_PrimitiveType(PrimitiveType::Unknown),
            m_MaterialIndex(-1)
		{

		}
	};

	struct MeshCreateParam
	{
        const aiScene* m_RawResource;
		int m_SubMeshCount;
		std::vector<std::shared_ptr<SubMeshCreateParam>> m_SubMeshCreateParam;
		MeshCreateParam() :
            m_RawResource(nullptr),
            m_SubMeshCount(0)
		{
		}

		~MeshCreateParam()
        {
            delete m_RawResource;
        }
	};


	class SubMesh
	{
	public:
		SubMesh() = delete;
		SubMesh(
			std::shared_ptr<VertexBuffer>& position,
			std::shared_ptr<VertexBuffer>& normal,
			std::shared_ptr<VertexBuffer>& texcoord,
			std::shared_ptr<IndexBuffer>& index,
			PrimitiveType pt,
            int materialIndex
            ):
			m_PositionBuffer(position),
			m_NormalBuffer(normal),
			m_TexCoordBuffer(texcoord),
			m_IndexBuffer(index),
			m_PrimitiveType(pt),
            m_MaterialIndex(materialIndex)
		{
		}

		virtual ~SubMesh() = default;

	public:
		std::shared_ptr<VertexBuffer> GetPositionBuffer()
		{
			return m_PositionBuffer;
		}

		std::shared_ptr<VertexBuffer> GetNormalBuffer()
		{
			return m_NormalBuffer;
		}

		std::shared_ptr<VertexBuffer> GetTexcoordBuffer()
		{
			return m_TexCoordBuffer;
		}

		std::shared_ptr<IndexBuffer> GetIndexBuffer()
		{
			return m_IndexBuffer;
		}

        int GetMaterialIndex() const
        {
            return m_MaterialIndex;
        }

	protected:
		std::shared_ptr<VertexBuffer> m_PositionBuffer = nullptr;
		std::shared_ptr<VertexBuffer> m_NormalBuffer = nullptr;
		std::shared_ptr<VertexBuffer> m_TexCoordBuffer = nullptr;
		std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
		PrimitiveType m_PrimitiveType = PrimitiveType::Unknown;
        int m_MaterialIndex;
	};

	class Mesh
	{
	public:
		virtual ~Mesh() = default;
	public:
		std::vector<std::shared_ptr<SubMesh>> m_SubMeshes;
	};




}


