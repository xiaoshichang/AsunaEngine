#include "../../Foundation/Math/AMath.h"
#include "DirectX11RenderItem.h"
#include "../Abstract/Renderer.h"

using namespace asuna;
using namespace std;


void DirectX11RenderItem::BindConstantBufferPerObject(DirectX11RenderContext* context) const
{
    auto perObject = GetConstantBufferPerObject();
    if (perObject != nullptr)
    {
        GetConstantBufferPerObject()->Bind();
    }
}


void DirectX11RenderItem::DrawMesh(DirectX11RenderContext* context, const std::shared_ptr<Material>& overrideMaterial) const
{
    auto mesh = GetMesh();
    for (const auto& subMesh : mesh->m_SubMeshes)
    {
        // set up material for mesh
        if (overrideMaterial == nullptr)
        {
            auto material = GetMaterial(subMesh->GetMaterialIndex());
            if (material == nullptr)
            {
                // use default material
                Logger::Warning("material not found!");
            }
            else
            {
                if (material->GetMaterialType() == MaterialType::MeshRender)
                {
                    material->SetMatrix("ModelMatrix", subMesh->GetModelMatrix());
                }
                material->Apply();
            }
        }
        else
        {
            overrideMaterial->Apply();
        }

        // set up mesh data
        auto positionBuffer = subMesh->GetPositionBuffer();
        auto normalBuffer = subMesh->GetNormalBuffer();
        auto texcoordBuffer = subMesh->GetTexcoordBuffer();
        auto indexBuffer = subMesh->GetIndexBuffer();

        if (positionBuffer != nullptr)
        {
            UINT stride = positionBuffer->GetStride();
            UINT offset = positionBuffer->GetOffset();
            auto vb = dynamic_pointer_cast<DirectX11VertexBuffer>(positionBuffer);
            auto buffer = vb->GetBuffer();
            // https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetvertexbuffers
            context->m_DeviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
        }

        if (normalBuffer != nullptr)
        {
            UINT stride = normalBuffer->GetStride();
            UINT offset = normalBuffer->GetOffset();
            auto vb = dynamic_pointer_cast<DirectX11VertexBuffer>(normalBuffer);
            auto buffer = vb->GetBuffer();
            context->m_DeviceContext->IASetVertexBuffers(1, 1, &buffer, &stride, &offset);
        }

        if (texcoordBuffer != nullptr)
        {
            UINT stride = texcoordBuffer->GetStride();
            UINT offset = texcoordBuffer->GetOffset();
            auto vb = dynamic_pointer_cast<DirectX11VertexBuffer>(texcoordBuffer);
            auto buffer = vb->GetBuffer();
            context->m_DeviceContext->IASetVertexBuffers(2, 1, &buffer, &stride, &offset);
        }

        // https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetprimitivetopology
        if (subMesh->GetPrimitiveType() == PrimitiveType::Triangle)
        {
            context->m_DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        }
        else if (subMesh->GetPrimitiveType() == PrimitiveType::Line)
        {
            context->m_DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
        }

        auto ib = dynamic_pointer_cast<DirectX11IndexBuffer>(indexBuffer);
        // https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetindexbuffer
        context->m_DeviceContext->IASetIndexBuffer(ib->GetBuffer(), ib->GetDXGIFormat(), 0);

        // https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-drawindexed
        context->m_DeviceContext->DrawIndexed(ib->GetElementCount(), ib->GetStartIndex(), 0);
    }
}

void DirectX11RenderItem::Render() const
{
    if (GetMesh() == nullptr || GetMaterial(0) == nullptr)
    {
        return;
    }

	auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Instance->GetContext()).get();
    BindConstantBufferPerObject(context);
    DrawMesh(context, nullptr);
}

void DirectX11RenderItem::Render(const shared_ptr<Material> &overrideMaterial) const
{
    auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Instance->GetContext()).get();
    BindConstantBufferPerObject(context);
    DrawMesh(context, overrideMaterial);
}

shared_ptr<DirectX11RenderItem> DirectX11RenderItem::Create(
        const std::shared_ptr<Mesh>& mesh,
        const vector<std::shared_ptr<Material>>& materials,
        const std::shared_ptr<ConstantBuffer>& perObject)
{
	return make_shared<DirectX11RenderItem>(mesh, materials, perObject);
}

shared_ptr<DirectX11RenderItem> DirectX11RenderItem::Create(
        const shared_ptr<Mesh> &mesh,
        const shared_ptr<ConstantBuffer> &perObject)
{
    return make_shared<DirectX11RenderItem>(mesh, perObject);
}
