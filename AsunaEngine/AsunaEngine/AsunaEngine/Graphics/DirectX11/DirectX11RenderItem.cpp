#include "DirectX11RenderItem.h"
#include "DirectX11RenderContext.h"
#include "../Renderer.h"

using namespace asuna;

void DirectX11RenderItem::Render()
{
	auto context = (DirectX11RenderContext*)Renderer::Current->GetContext();

	context->m_DeviceContext->IASetInputLayout(m_VertexShader->m_Layout);
	context->m_DeviceContext->VSSetShader(m_VertexShader->m_VS, 0, 0);
	context->m_DeviceContext->PSSetShader(m_PixelShader->m_PS, 0, 0);

	UINT stride = 7 * sizeof(float);
	UINT offset = 0;
	// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetvertexbuffers
	context->m_DeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer->m_Buffer, &stride, &offset);
	context->m_DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->m_DeviceContext->Draw(3, 0);
}
