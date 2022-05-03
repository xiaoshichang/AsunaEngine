#include "../../Foundation/Platform/Assert.h"
#include "../Abstract/Renderer.h"
#include "DirectX11IndexBuffer.h"
#include "DirectX11RenderContext.h"
using namespace asuna;
using namespace std;

DXGI_FORMAT asuna::DirectX11IndexBuffer::GetDXGIFormat()
{
	if (m_Format == IndexBufferFormat::UINT16)
	{
		return DXGI_FORMAT_R16_UINT;
	}
	else if (m_Format == IndexBufferFormat::UINT32)
	{
		return DXGI_FORMAT_R32_UINT;
	}
	else
	{
		ASUNA_ASSERT(false);
		return DXGI_FORMAT_UNKNOWN;
	}
}

shared_ptr<DirectX11IndexBuffer> DirectX11IndexBuffer::Create(shared_ptr<IndexBufferCreateParam> param)
{
	ID3D11Buffer* buffer;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = param->m_ElementCount * param->GetFormatSize();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = param->m_IndexData;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	HRESULT result;
	auto context = dynamic_pointer_cast<DirectX11RenderContext>( Renderer::Current->GetContext());
	result = context->m_Device->CreateBuffer(&indexBufferDesc, &indexData, &buffer);
	ASUNA_ASSERT(SUCCEEDED(result));

	auto indexBuffer = make_shared<DirectX11IndexBuffer>(buffer, param);
	return indexBuffer;
}
