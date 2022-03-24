#include "DirectX11Mesh.h"
using namespace asuna;

DirectX11Mesh* DirectX11Mesh::Create(MeshCreateParam param)
{
	auto mesh = new DirectX11Mesh();
	mesh->m_VertexBuffer = DirectX11VertexBuffer::Create(param.m_VertexBufferCreateParam);
	mesh->m_IndexBuffer = DirectX11IndexBuffer::Create(param.m_IndexBufferCreateParam);
	return mesh;
}
