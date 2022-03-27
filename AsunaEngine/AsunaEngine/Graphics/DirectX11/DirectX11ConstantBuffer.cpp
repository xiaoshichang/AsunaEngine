#include "DirectX11ConstantBuffer.h"
#include "DirectX11RenderContext.h"
#include "../Renderer.h"
#include "../../Foundation/Platform/Assert.h"
using namespace asuna;
using namespace std;

shared_ptr<DirectX11ConstantBuffer> DirectX11ConstantBuffer::Create()
{
	auto constantBuffer = make_shared<DirectX11ConstantBuffer>();

	D3D11_BUFFER_DESC matrixBufferDesc;
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(ConstantBufferData);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Current->GetContext());
	auto result = context->m_Device->CreateBuffer(&matrixBufferDesc, NULL, &constantBuffer->m_ConstantBuffer);
	ASUNA_ASSERT(SUCCEEDED(result));

	return constantBuffer;
}
