#pragma once
#include <memory>
#include <vector>
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include <assimp/scene.h>
#include "../../Foundation/Math/AMath.h"
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
        Matrix4x4f m_ModelMatrix;

		SubMeshCreateParam() : 
			m_PositionCreateParam(nullptr),
			m_NormalCreateParam(nullptr),
			m_TexcoordCreateParam(nullptr),
			m_IndexCreateParam(nullptr),
			m_PrimitiveType(PrimitiveType::Unknown),
            m_MaterialIndex(-1),
            m_ModelMatrix()
		{

		}
	};

	struct MeshCreateParam
	{
        const aiScene* m_RawResource;
		std::vector<std::shared_ptr<SubMeshCreateParam>> m_SubMeshCreateParam;
		MeshCreateParam() :
            m_RawResource(nullptr)
		{
		}

		~MeshCreateParam()
        {
            delete m_RawResource;
        }

        int GetSubMeshCunt() const
        {
            return m_SubMeshCreateParam.size();
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
            const std::shared_ptr<SubMeshCreateParam>& param
            ):
			m_PositionBuffer(position),
			m_NormalBuffer(normal),
			m_TexCoordBuffer(texcoord),
			m_IndexBuffer(index),
			m_PrimitiveType(param->m_PrimitiveType),
            m_MaterialIndex(param->m_MaterialIndex),
            m_ModelMatrix(param->m_ModelMatrix)
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

        Matrix4x4f GetModelMatrix() const
        {
            return m_ModelMatrix;
        }


        PrimitiveType GetPrimitiveType() const { return m_PrimitiveType;}

	protected:
		std::shared_ptr<VertexBuffer> m_PositionBuffer = nullptr;
		std::shared_ptr<VertexBuffer> m_NormalBuffer = nullptr;
		std::shared_ptr<VertexBuffer> m_TexCoordBuffer = nullptr;
		std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
		PrimitiveType m_PrimitiveType = PrimitiveType::Unknown;
        int m_MaterialIndex;
        Matrix4x4f m_ModelMatrix;
	};

	class Mesh
	{
	public:
		virtual ~Mesh() = default;
	public:
		std::vector<std::shared_ptr<SubMesh>> m_SubMeshes;
	};




}


