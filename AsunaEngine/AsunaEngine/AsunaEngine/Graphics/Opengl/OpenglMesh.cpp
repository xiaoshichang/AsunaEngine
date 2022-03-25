
#include <cstdint>

#include "../../Foundation/Platform/Assert.h"
#include "glad/glad.h"
#include "OpenglMesh.h"
using namespace asuna;


unsigned int OpengSubMesh::GetGLPrimitive()
{
	if (m_PrimitiveType == PrimitiveType::Triangle)
	{
		return GL_TRIANGLES;
	}
	else if (m_PrimitiveType == PrimitiveType::Line)
	{
		return GL_LINES;
	}
	else if (m_PrimitiveType == PrimitiveType::Point)
	{
		return GL_POINTS;
	}
	else
	{
		ASUNA_ASSERT(false);
		return 0;
	}
}

OpengSubMesh* OpengSubMesh::Create(SubMeshCreateParam* param)
{
	auto mesh = new OpengSubMesh();
	mesh->m_PrimitiveType = param->m_PrimitiveType;

	if (param->m_PositionCreateParam != nullptr)
	{
		mesh->m_PositionBuffer = OpenglVertexBuffer::Create(param->m_PositionCreateParam);
	}
	if (param->m_NormalCreateParam != nullptr)
	{
		mesh->m_NormalBuffer = OpenglVertexBuffer::Create(param->m_NormalCreateParam);
	}
	if (param->m_TexcoordCreateParam != nullptr)
	{
		mesh->m_TexCoordBuffer = OpenglVertexBuffer::Create(param->m_TexcoordCreateParam);
	}
	if (param->m_TexcoordCreateParam != nullptr)
	{
		mesh->m_IndexBuffer = OpenglIndexBuffer::Create(param->m_IndexCreateParam);
	}

	// bind vao
	{
		// Allocate an OpenGL vertex array object.
		glGenVertexArrays(1, &mesh->m_VertexArray);
		// Bind the vertex array object to store all the buffers and vertex attributes we create here.
		glBindVertexArray(mesh->m_VertexArray);
		if (mesh->m_PositionBuffer != nullptr)
		{
			auto vb = (OpenglVertexBuffer*)mesh->m_PositionBuffer;
			vb->Bind(0);
		}
		if (mesh->m_NormalBuffer != nullptr)
		{
			auto vb = (OpenglVertexBuffer*)mesh->m_NormalBuffer;
			vb->Bind(1);
		}
		if (mesh->m_TexCoordBuffer != nullptr)
		{
			auto vb = (OpenglVertexBuffer*)mesh->m_TexCoordBuffer;
			vb->Bind(2);
		}
		if (mesh->m_IndexBuffer != nullptr)
		{
			auto ib = (OpenglIndexBuffer*)mesh->m_IndexBuffer;
			ib->Bind();
		}
		glBindVertexArray(0);
	}
	return mesh;
}


OpenglMesh* OpenglMesh::Create(MeshCreateParam* param)
{
	auto mesh = new OpenglMesh();
	for (size_t i = 0; i < param->m_SubMeshCount; i++)
	{
		auto subPram = param->m_SubMeshCreateParam[i];
		auto subMesh = OpengSubMesh::Create(&subPram);
		mesh->m_SubMeshes.push_back(subMesh);
	}
	return mesh;
}
