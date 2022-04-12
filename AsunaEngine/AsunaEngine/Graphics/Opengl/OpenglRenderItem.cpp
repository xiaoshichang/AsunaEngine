#include "OpenglRenderItem.h"
#include "../Abstract/Renderer.h"
#include "../../Foundation/Math/AMath.h"
#include "../../3rd/Glad/include/glad/glad.h"

using namespace asuna;
using namespace std;

void OpenglRenderItem::Render()
{
    m_ConstantBufferPerObject->Bind();
	for (auto & element : m_Mesh->m_SubMeshes)
	{
		auto mesh = dynamic_pointer_cast<OpenglSubMesh>(element);
        glBindVertexArray(mesh->m_VAO);
        auto indexBuffer = dynamic_pointer_cast<OpenglIndexBuffer>(mesh->GetIndexBuffer());
        indexBuffer->Bind();
        auto material = GetMaterial(mesh->GetMaterialIndex());
        material->Apply();
		glDrawElements(mesh->GetGLPrimitive(), indexBuffer->GetElementCount(), indexBuffer->GetGLIndexType(), nullptr);
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
