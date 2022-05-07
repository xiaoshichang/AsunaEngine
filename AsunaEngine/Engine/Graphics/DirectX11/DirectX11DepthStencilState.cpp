//
// Created by xiao on 2022/4/11.
//

#include "DirectX11DepthStencilState.h"
#include "DirectX11RenderContext.h"
#include "../Abstract/Renderer.h"

using namespace asuna;
using namespace std;

DirectX11DepthStencilState::DirectX11DepthStencilState() :
        m_State(nullptr)
{
}

DirectX11DepthStencilState::~DirectX11DepthStencilState()
{
    if (m_State != nullptr)
    {
        m_State->Release();
        m_State = nullptr;
    }
}


void DirectX11DepthStencilState::Bind()
{
    if (m_Dirty)
    {
        RebuildState();
    }
    auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Instance->GetContext()).get();
    context->m_DeviceContext->OMSetDepthStencilState(m_State, 1);
}

void DirectX11DepthStencilState::RebuildState()
{
    if (m_State != nullptr)
    {
        m_State->Release();
        m_State = nullptr;
    }

    D3D11_DEPTH_STENCIL_DESC dsDesc;
    // Depth
    dsDesc.DepthEnable = m_DepthEnable;
    dsDesc.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>(m_DepthWriteMask);
    dsDesc.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(m_DepthComparisonFunc);
    // Stencil test parameters
    dsDesc.StencilEnable = m_StencilEnable;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    // Stencil operations if pixel is back-facing
    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    // Create depth stencil state
    auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Instance->GetContext()).get();
    context->m_Device->CreateDepthStencilState(&dsDesc, &m_State);
    m_Dirty = false;
}
