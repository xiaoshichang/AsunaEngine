#include "../../Foundation/Math/AMath.h"
#include "DirectX11RenderItem.h"
#include "DirectX11RenderContext.h"
#include "DirectX11ConstantBuffer.h"
#include "DirectX11Shader.h"
#include "../Renderer.h"

using namespace asuna;
using namespace std;


void DirectX11RenderItem::BindConstantBufferPerObject(DirectX11RenderContext* context)
{
    auto cb = dynamic_pointer_cast<DirectX11ConstantBuffer>(GetConstantBufferPerObject());
    auto data = (ConstantBufferDataPerObject*)cb->GetData();
    auto buffer = cb->GetBuffer();
    // map data
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    auto result = context->m_DeviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    ASUNA_ASSERT(SUCCEEDED(result));
    auto dataPtr = (ConstantBufferDataPerObject*)mappedResource.pData;
    memcpy(dataPtr, data, sizeof(ConstantBufferDataPerObject));
    context->m_DeviceContext->Unmap(buffer, 0);
    context->m_DeviceContext->VSSetConstantBuffers(1, 1, &buffer);
    context->m_DeviceContext->PSSetConstantBuffers(1, 1, &buffer);
}

void DirectX11RenderItem::BindConstantBufferPerScene(DirectX11RenderContext* context)
{
    auto cb = dynamic_pointer_cast<DirectX11ConstantBuffer>(GetConstantBufferPerScene());
    auto data = (ConstantBufferDataPerScene*)cb->GetData();
    auto buffer = cb->GetBuffer();
    // map data
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    auto result = context->m_DeviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    ASUNA_ASSERT(SUCCEEDED(result));
    auto dataPtr = (ConstantBufferDataPerScene*)mappedResource.pData;
    memcpy(dataPtr, data, sizeof(ConstantBufferDataPerScene));
    context->m_DeviceContext->Unmap(buffer, 0);
    context->m_DeviceContext->VSSetConstantBuffers(0, 1, &buffer);
    context->m_DeviceContext->PSSetConstantBuffers(0, 1, &buffer);
}

void DirectX11RenderItem::BindShaders(DirectX11RenderContext* context)
{
    auto vs = dynamic_pointer_cast<DirectX11VextexShader>(GetVertexShader());
    context->m_DeviceContext->IASetInputLayout(vs->GetLayout());
    context->m_DeviceContext->VSSetShader(vs->GetShader(), 0, 0);
    auto ps = dynamic_pointer_cast<DirectX11PixelShader>(GetPixelShader());
    context->m_DeviceContext->PSSetShader(ps->GetShader(), 0, 0);
}

void DirectX11RenderItem::DrawMesh(DirectX11RenderContext* context)
{
    auto mesh = GetMesh();
    for (const auto& subMesh : mesh->m_SubMeshes)
    {
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
        context->m_DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        auto ib = dynamic_pointer_cast<DirectX11IndexBuffer>(indexBuffer);
        // https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetindexbuffer
        context->m_DeviceContext->IASetIndexBuffer(ib->GetBuffer(), ib->GetDXGIFormat(), 0);

        // https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-drawindexed
        context->m_DeviceContext->DrawIndexed(ib->GetElementCount(), 0, 0);
    }
}

void DirectX11RenderItem::Render()
{
    if (GetMesh() == nullptr || GetVertexShader() == nullptr || GetPixelShader() == nullptr)
    {
        return;
    }

	auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Current->GetContext()).get();
    BindConstantBufferPerScene(context);
    BindConstantBufferPerObject(context);
    BindShaders(context);
    DrawMesh(context);
}

std::shared_ptr<DirectX11RenderItem> asuna::DirectX11RenderItem::Create(
        const std::shared_ptr<Mesh>& mesh,
        const std::shared_ptr<Shader>& vs,
        const std::shared_ptr<Shader>& ps,
        const std::shared_ptr<ConstantBuffer>& perObject,
        const std::shared_ptr<ConstantBuffer>& perScene
        )
{
	return make_shared<DirectX11RenderItem>(mesh, vs, ps, perObject, perScene);
}






