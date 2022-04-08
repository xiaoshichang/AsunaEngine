#include "DirectX11ConstantBuffer.h"
#include "DirectX11RenderContext.h"
#include "../Abstract/Renderer.h"
#include "../../Foundation/Platform/Assert.h"

using namespace asuna;
using namespace std;

shared_ptr<DirectX11ConstantBuffer> DirectX11ConstantBuffer::Create(ConstantBufferDataType dt)
{
	ID3D11Buffer* cb;
	D3D11_BUFFER_DESC constantBufferDesc;
	// Set up the description of the dynamic matrix constant buffer that is in the vertex shader.
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.ByteWidth = ConstantBuffer::GetDataSizeByDataType(dt);
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantBufferDesc.MiscFlags = 0;
    constantBufferDesc.StructureByteStride = 0;

	auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Current->GetContext());
	auto result = context->m_Device->CreateBuffer(&constantBufferDesc, NULL, &cb);
	ASUNA_ASSERT(SUCCEEDED(result));

	return 	make_shared<DirectX11ConstantBuffer>(dt, cb);
	;
}
