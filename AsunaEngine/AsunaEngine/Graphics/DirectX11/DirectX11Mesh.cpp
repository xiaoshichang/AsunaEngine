#include "DirectX11Mesh.h"
using namespace asuna;
using namespace std;

 shared_ptr<DirectX11SubMesh> DirectX11SubMesh::Create(shared_ptr<SubMeshCreateParam> param)
{
	auto submesh = make_shared<DirectX11SubMesh>();
	submesh->m_PrimitiveType = param->m_PrimitiveType;
	if (param->m_PositionCreateParam != nullptr)
	{
		submesh->m_PositionBuffer = DirectX11VertexBuffer::Create(param->m_PositionCreateParam);
	}

	if (param->m_NormalCreateParam != nullptr)
	{
		submesh->m_NormalBuffer = DirectX11VertexBuffer::Create(param->m_NormalCreateParam);
	}

	if (param->m_TexcoordCreateParam != nullptr)
	{
		submesh->m_TexCoordBuffer = DirectX11VertexBuffer::Create(param->m_TexcoordCreateParam);
	}

	if (param->m_IndexCreateParam != nullptr)
	{
		submesh->m_IndexBuffer = DirectX11IndexBuffer::Create(param->m_IndexCreateParam);
	}
	return submesh;
}

shared_ptr<DirectX11Mesh> DirectX11Mesh::Create(shared_ptr<MeshCreateParam> param)
{
	auto mesh = make_shared<DirectX11Mesh>();
	for (size_t i = 0; i < param->m_SubMeshCount; i++)
	{
		auto submesh = DirectX11SubMesh::Create(param->m_SubMeshCreateParam[i]);
		mesh->m_SubMeshes.push_back(submesh);
	}
	return mesh;
}


