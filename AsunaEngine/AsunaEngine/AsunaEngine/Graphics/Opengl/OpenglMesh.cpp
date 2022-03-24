
#include <cstdint>

#include "glad/glad.h"
#include "OpenglMesh.h"
using namespace asuna;



OpenglMesh* OpenglMesh::Create(MeshCreateParam param)
{
	auto vertexBuffer = OpenglVertexBuffer::Create(param.m_VertexBufferCreateParam);
	auto indexBuffer = OpenglIndexBuffer::Create(param.m_IndexBufferCreateParam);
	auto mesh = new OpenglMesh();
	mesh->m_VertexBuffer = vertexBuffer;
	mesh->m_IndexBuffer = indexBuffer;

	// Allocate an OpenGL vertex array object.
	glGenVertexArrays(1, &mesh->m_VertexArray);
	// Bind the vertex array object to store all the buffers and vertex attributes we create here.
	glBindVertexArray(mesh->m_VertexArray);
	vertexBuffer->Bind();
	indexBuffer->Bind();
	glBindVertexArray(0);
	return mesh;
}

