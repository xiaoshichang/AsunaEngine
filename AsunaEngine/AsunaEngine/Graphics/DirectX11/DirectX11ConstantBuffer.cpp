#include "DirectX11ConstantBuffer.h"
#include "DirectX11RenderContext.h"
#include "../Abstract/Renderer.h"
#include "../../Foundation/Platform/Assert.h"

using namespace asuna;
using namespace std;

shared_ptr<DirectX11ConstantBuffer> DirectX11ConstantBuffer::Create(ConstantBufferDataType dt, int size)
{
	ID3D11Buffer* cb;
	D3D11_BUFFER_DESC constantBufferDesc;
	// Set up the description of the dynamic matrix constant buffer that is in the vertex shader.
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.ByteWidth = size;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantBufferDesc.MiscFlags = 0;
    constantBufferDesc.StructureByteStride = 0;

	auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Current->GetContext());
	auto result = context->m_Device->CreateBuffer(&constantBufferDesc, nullptr, &cb);
	ASUNA_ASSERT(SUCCEEDED(result));

	return 	make_shared<DirectX11ConstantBuffer>(dt, size, cb);
}

void DirectX11ConstantBuffer::Bind()
{
    auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Current->GetContext()).get();
    // map data
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    auto result = context->m_DeviceContext->Map(m_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    ASUNA_ASSERT(SUCCEEDED(result));

    auto dataPtr = (void*)mappedResource.pData;
    memcpy(dataPtr, m_Data, m_Size);
    context->m_DeviceContext->Unmap(m_ConstantBuffer, 0);

    if (GetDataType() == ConstantBufferDataType::PerFrame)
    {
        context->m_DeviceContext->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
    }
    else if(GetDataType() == ConstantBufferDataType::PerObject)
    {
        context->m_DeviceContext->VSSetConstantBuffers(1, 1, &m_ConstantBuffer);
    }
    else if(GetDataType() == ConstantBufferDataType::PerMaterial)
    {
        context->m_DeviceContext->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);
    }
    else
    {
        ASUNA_ASSERT(false);
    }
}
