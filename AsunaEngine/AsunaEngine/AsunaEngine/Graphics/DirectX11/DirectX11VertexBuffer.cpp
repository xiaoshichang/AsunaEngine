#include "../../Foundation/Platform/Assert.h"
#include "../Renderer.h"
#include "DirectX11VertexBuffer.h"
#include "DirectX11RenderContext.h"
using namespace asuna;
using namespace std;

shared_ptr<DirectX11VertexBuffer> DirectX11VertexBuffer::Create(shared_ptr<VertexBufferCreateParam> param)
{
	HRESULT result;
	auto vertexBuffer = make_shared<DirectX11VertexBuffer>();
	vertexBuffer->m_ElementCount = param->m_ElementCount;
	vertexBuffer->m_Format = param->m_Format;
	vertexBuffer->m_Stride = param->GetFormatStride();
	vertexBuffer->m_Offset = 0;

	auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Current->GetContext());
	auto sizeInBytes = param->m_ElementCount * param->GetFormatStride();

	// create the vertex buffer
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_usage
		// D3D11_USAGE_DYNAMIC is typically used on resources with vertex data and on constant buffers.
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeInBytes;						// Size of the buffer in bytes.
		// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_bind_flag
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_cpu_access_flag
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// allow CPU to write in buffer
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		result = context->m_Device->CreateBuffer(&bd, NULL, &vertexBuffer->m_Buffer);
		ASUNA_ASSERT(SUCCEEDED(result));
	}
	// copy the vertices into the buffer
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		result = context->m_DeviceContext->Map(vertexBuffer->m_Buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
		ASUNA_ASSERT(SUCCEEDED(result));
		memcpy(ms.pData, param->m_VertexData, sizeInBytes);                       // copy the data
		context->m_DeviceContext->Unmap(vertexBuffer->m_Buffer, NULL);
	}
	return vertexBuffer;
}
