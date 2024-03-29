
#include <cstdint>

#include "../../Foundation/Platform/Assert.h"
#include "../../3rd/Glad/include/glad/glad.h"
#include "OpenglMesh.h"

using namespace asuna;
using namespace std;

asuna::OpenglSubMesh::~OpenglSubMesh()
{
	if (m_VAO != 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}

unsigned int OpenglSubMesh::GetGLPrimitive()
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

std::shared_ptr<OpenglSubMesh> OpenglSubMesh::Create(const std::shared_ptr<SubMeshCreateParam>& param)
{
	std::shared_ptr<VertexBuffer> position = nullptr;
	std::shared_ptr<VertexBuffer> normal = nullptr;
	std::shared_ptr<VertexBuffer> texcoord = nullptr;
	std::shared_ptr<IndexBuffer>  index = nullptr;
	unsigned int vao;

	if (param->m_PositionCreateParam != nullptr)
	{
		position = OpenglVertexBuffer::Create(param->m_PositionCreateParam);
	}
	if (param->m_NormalCreateParam != nullptr)
	{
		normal = OpenglVertexBuffer::Create(param->m_NormalCreateParam);
	}
	if (param->m_TexcoordCreateParam != nullptr)
	{
		texcoord = OpenglVertexBuffer::Create(param->m_TexcoordCreateParam);
	}
	if (param->m_IndexCreateParam != nullptr)
	{
		index = OpenglIndexBuffer::Create(param->m_IndexCreateParam);
	}

	// bind vao
	{
		// Allocate an OpenGL vertex array object.
		glGenVertexArrays(1, &vao);
		// Bind the vertex array object to store all the buffers and vertex attributes we create here.
		glBindVertexArray(vao);
        int available = 0;
		if (position != nullptr)
		{
			auto vb = dynamic_pointer_cast<OpenglVertexBuffer>(position);
			vb->Bind(available);
            available++;
		}
		if (normal != nullptr)
		{
			auto vb = dynamic_pointer_cast<OpenglVertexBuffer>(normal);
            vb->Bind(available);
            available++;
		}
		if (texcoord != nullptr)
		{
			auto vb = dynamic_pointer_cast<OpenglVertexBuffer>(texcoord);
            vb->Bind(available);
            available++;
		}
		if (index != nullptr)
		{
			auto ib = dynamic_pointer_cast<OpenglIndexBuffer>(index);
			ib->Bind();
		}
		glBindVertexArray(0);
	}
	return make_shared<OpenglSubMesh>(vao, position, normal, texcoord, index, param);
}


shared_ptr<OpenglMesh> OpenglMesh::Create(const shared_ptr<MeshCreateParam>& param)
{
	auto mesh = make_shared<OpenglMesh>(param);
	for (size_t i = 0; i < param->GetSubMeshCunt(); i++)
	{
		auto subPram = param->m_SubMeshCreateParam[i];
		auto subMesh = OpenglSubMesh::Create(subPram);
		mesh->m_SubMeshes.push_back(subMesh);
	}
	return mesh;
}
