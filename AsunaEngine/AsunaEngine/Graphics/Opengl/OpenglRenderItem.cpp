#include "OpenglRenderItem.h"
#include "../Abstract/Renderer.h"
#include "../../Foundation/Math/AMath.h"
#include "../../3rd/Glad/include/glad/glad.h"

using namespace asuna;
using namespace std;

void OpenglRenderItem::Render()
{
    if (m_ConstantBufferPerObject != nullptr)
    {
        m_ConstantBufferPerObject->Bind();
    }
	for (auto & element : m_Mesh->m_SubMeshes)
	{
		auto mesh = dynamic_pointer_cast<OpenglSubMesh>(element);
        glBindVertexArray(mesh->m_VAO);
        auto indexBuffer = dynamic_pointer_cast<OpenglIndexBuffer>(mesh->GetIndexBuffer());
        indexBuffer->Bind();
        auto material = GetMaterial(mesh->GetMaterialIndex());
        material->Apply();
        int start = indexBuffer->GetStartIndex();
        int count = indexBuffer->GetElementCount();
        int end = start + count - 1;
        glDrawRangeElements(mesh->GetGLPrimitive(), start, end, count, indexBuffer->GetGLIndexType(), nullptr);
	}
	
}

shared_ptr<OpenglRenderItem> asuna::OpenglRenderItem::Create(
        const shared_ptr<Mesh>& mesh,
        const vector<shared_ptr<Material>>& materials,
        const shared_ptr<ConstantBuffer>& perObject)
{
	return make_shared<OpenglRenderItem>(mesh, materials, perObject);
}

std::shared_ptr<OpenglRenderItem> OpenglRenderItem::Create(
        const shared_ptr<Mesh>& mesh,
        const shared_ptr<ConstantBuffer>& perObject)
{
    return make_shared<OpenglRenderItem>(mesh, perObject);
}
