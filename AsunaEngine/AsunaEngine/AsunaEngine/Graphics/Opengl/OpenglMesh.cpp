
#include <cstdint>

#include "../../Foundation/Platform/Assert.h"
#include "glad/glad.h"
#include "OpenglMesh.h"

using namespace asuna;
using namespace std;

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

std::shared_ptr<OpengSubMesh> OpengSubMesh::Create(std::shared_ptr<SubMeshCreateParam> param)
{
	auto mesh = make_shared<OpengSubMesh>();
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
			auto vb = dynamic_pointer_cast<OpenglVertexBuffer>(mesh->m_PositionBuffer);
			vb->Bind(0);
		}
		if (mesh->m_NormalBuffer != nullptr)
		{
			auto vb = dynamic_pointer_cast<OpenglVertexBuffer>(mesh->m_NormalBuffer);
			vb->Bind(1);
		}
		if (mesh->m_TexCoordBuffer != nullptr)
		{
			auto vb = dynamic_pointer_cast<OpenglVertexBuffer>(mesh->m_TexCoordBuffer);
			vb->Bind(2);
		}
		if (mesh->m_IndexBuffer != nullptr)
		{
			auto ib = dynamic_pointer_cast<OpenglIndexBuffer>(mesh->m_IndexBuffer);
			ib->Bind();
		}
		glBindVertexArray(0);
	}
	return mesh;
}


shared_ptr<OpenglMesh> OpenglMesh::Create(shared_ptr<MeshCreateParam> param)
{
	auto mesh = make_shared<OpenglMesh>();
	for (size_t i = 0; i < param->m_SubMeshCount; i++)
	{
		auto subPram = param->m_SubMeshCreateParam[i];
		auto subMesh = OpengSubMesh::Create(subPram);
		mesh->m_SubMeshes.push_back(subMesh);
	}
	return mesh;
}
