#include "DirectX11RasterizationState.h"
#include "DirectX11RenderContext.h"
#include "../Abstract/Renderer.h"

using namespace asuna;
using namespace std;

DirectX11RasterizationState::DirectX11RasterizationState(const RasterizationStateDesc& desc) :
	RasterizationState(desc)
{
	D3D11_RASTERIZER_DESC rasterizerState = ConvertDesc(desc);
	auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Instance->GetContext());
	auto hr = context->m_Device->CreateRasterizerState(&rasterizerState, &m_State);
	ASUNA_ASSERT(SUCCEEDED(hr));
}

DirectX11RasterizationState::~DirectX11RasterizationState()
{
	if (m_State != nullptr)
	{
		m_State->Release();
		m_State = nullptr;
	}
}

D3D11_RASTERIZER_DESC DirectX11RasterizationState::ConvertDesc(const RasterizationStateDesc& desc)
{
	D3D11_RASTERIZER_DESC rasterizerState;

	if (desc.FillMode == RasterizationFillMode::Solid)
	{
		rasterizerState.FillMode = D3D11_FILL_SOLID;
	}
	else if (desc.FillMode == RasterizationFillMode::WireFrame)
	{
		rasterizerState.FillMode = D3D11_FILL_WIREFRAME;
	}
	else
	{
		ASUNA_ASSERT(false);
	}

	if (desc.CullMode == RasterizationCullMode::None)
	{
		rasterizerState.CullMode = D3D11_CULL_NONE;
	}
	else if (desc.CullMode == RasterizationCullMode::Front)
	{
		rasterizerState.CullMode = D3D11_CULL_FRONT;
	}
	else if (desc.CullMode == RasterizationCullMode::Back)
	{
		rasterizerState.CullMode = D3D11_CULL_BACK;
	}
	else
	{
		ASUNA_ASSERT(false);
	}
	
	rasterizerState.FrontCounterClockwise = desc.FrontCounterClockWise;

	rasterizerState.DepthBias = false;
	rasterizerState.DepthBiasClamp = 0;
	rasterizerState.SlopeScaledDepthBias = 0;
	rasterizerState.DepthClipEnable = true;
	rasterizerState.ScissorEnable = false;
	rasterizerState.MultisampleEnable = false;
	rasterizerState.AntialiasedLineEnable = false;
	return rasterizerState;
}
