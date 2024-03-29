#include "DirectX11Mesh.h"
using namespace asuna;
using namespace std;

 shared_ptr<DirectX11SubMesh> DirectX11SubMesh::Create(const shared_ptr<SubMeshCreateParam>& param)
{
	 std::shared_ptr<VertexBuffer> position = nullptr;
	 std::shared_ptr<VertexBuffer> normal = nullptr;
	 std::shared_ptr<VertexBuffer> texcoord = nullptr;
	 std::shared_ptr<IndexBuffer>  index = nullptr;

	if (param->m_PositionCreateParam != nullptr)
	{
		position = DirectX11VertexBuffer::Create(param->m_PositionCreateParam);
	}

	if (param->m_NormalCreateParam != nullptr)
	{
		normal = DirectX11VertexBuffer::Create(param->m_NormalCreateParam);
	}

	if (param->m_TexcoordCreateParam != nullptr)
	{
		texcoord = DirectX11VertexBuffer::Create(param->m_TexcoordCreateParam);
	}

	if (param->m_IndexCreateParam != nullptr)
	{
		index = DirectX11IndexBuffer::Create(param->m_IndexCreateParam);
	}
	return make_shared<DirectX11SubMesh>(position, normal, texcoord, index, param);
}

shared_ptr<DirectX11Mesh> DirectX11Mesh::Create(const shared_ptr<MeshCreateParam>& param)
{
	auto mesh = make_shared<DirectX11Mesh>(param);
	for (size_t i = 0; i < param->GetSubMeshCunt(); i++)
	{
		auto sub = DirectX11SubMesh::Create(param->m_SubMeshCreateParam[i]);
		mesh->m_SubMeshes.push_back(sub);
	}
	return mesh;
}


