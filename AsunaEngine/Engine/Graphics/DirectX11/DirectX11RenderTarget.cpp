#include "DirectX11RenderTarget.h"
#include "DirectX11RenderContext.h"
#include "DirectX11Renderer.h"
#include "../../Foundation/Platform/Assert.h"
using namespace asuna;
using namespace std;

DirectX11RenderTarget::DirectX11RenderTarget(RenderTargetDesc desc, ID3D11Device* device) : RenderTarget(desc)
{
    CreateRenderTarget(desc, device);
    CreateDepthStencilResource(desc, device);
}

DirectX11RenderTarget::DirectX11RenderTarget(RenderTargetDesc desc, ID3D11Device* device, IDXGISwapChain *swapChain) : RenderTarget(desc)
{
    CreateRenderTargetFromSwapChain(desc, device, swapChain);
    CreateDepthStencilResource(desc, device);
}

asuna::DirectX11RenderTarget::~DirectX11RenderTarget()
{
    ClearRenderTarget();
    ClearDepthStencilResource();
}

void DirectX11RenderTarget::ClearRenderTarget()
{
    if (m_RenderTargetView != nullptr)
    {
        m_RenderTargetView->Release();
        m_RenderTargetView = nullptr;
    }

    if (m_RenderTargetTexture != nullptr)
    {
        m_RenderTargetTexture->Release();
        m_RenderTargetTexture = nullptr;
    }

    if (m_ShaderResourceView != nullptr)
    {
        m_ShaderResourceView->Release();
        m_ShaderResourceView = nullptr;
    }
}

void DirectX11RenderTarget::ClearDepthStencilResource()
{
    if (m_DepthStencilTexture != nullptr)
    {
        m_DepthStencilTexture->Release();
        m_DepthStencilTexture = nullptr;
    }
    if (m_DepthStencilView != nullptr)
    {
        m_DepthStencilView->Release();
        m_DepthStencilView = nullptr;
    }
}

void DirectX11RenderTarget::CreateRenderTarget(RenderTargetDesc desc, ID3D11Device* device)
{
    if (desc.usage == RenderTargetUsage::Default)
    {
        D3D11_TEXTURE2D_DESC textureDesc;
        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
        D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
        // render texture
        ZeroMemory(&textureDesc, sizeof(textureDesc));
        textureDesc.Width = desc.width;
        textureDesc.Height = desc.height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;
        auto result = device->CreateTexture2D(&textureDesc, nullptr, &m_RenderTargetTexture);
        ASUNA_ASSERT(SUCCEEDED(result));
        // render target view
        renderTargetViewDesc.Format = textureDesc.Format;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        renderTargetViewDesc.Texture2D.MipSlice = 0;
        result = device->CreateRenderTargetView(m_RenderTargetTexture, &renderTargetViewDesc, &m_RenderTargetView);
        ASUNA_ASSERT(SUCCEEDED(result));
        // shader resource view
        shaderResourceViewDesc.Format = textureDesc.Format;
        shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
        shaderResourceViewDesc.Texture2D.MipLevels = 1;
        result = device->CreateShaderResourceView(m_RenderTargetTexture, &shaderResourceViewDesc, &m_ShaderResourceView);
    }
    else if (desc.usage == RenderTargetUsage::ShadowMap)
    {
        m_RenderTargetTexture = nullptr;
        m_RenderTargetView = nullptr;
        m_ShaderResourceView = nullptr;
    }
    else
    {
        ASUNA_ASSERT(false);
    }
}

void DirectX11RenderTarget::CreateRenderTargetFromSwapChain(RenderTargetDesc desc, ID3D11Device *device, IDXGISwapChain* swapChain)
{
    if (desc.usage == RenderTargetUsage::Default)
    {
        // Get the pointer to the back buffer.
        ID3D11Texture2D* backBufferPtr;
        auto result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
        ASUNA_ASSERT(result >= 0);
        // Create the render target view with the back buffer pointer.
        result = device->CreateRenderTargetView(backBufferPtr, nullptr, &m_RenderTargetView);
        ASUNA_ASSERT(result >= 0);
        // Release pointer to the back buffer as we no longer need it.
        backBufferPtr->Release();
        backBufferPtr = nullptr;
        m_RenderTargetTexture = nullptr;
        m_ShaderResourceView = nullptr;
    }
    else
    {
        ASUNA_ASSERT(false);
    }
}

shared_ptr<DirectX11RenderTarget> DirectX11RenderTarget::CreateFromSwapChain(
        RenderTargetDesc desc, 
        ID3D11Device* device, 
        IDXGISwapChain* swapChain)
{
    return make_shared<DirectX11RenderTarget>(desc, device, swapChain);
}

shared_ptr<DirectX11RenderTarget> asuna::DirectX11RenderTarget::Create(RenderTargetDesc desc, ID3D11Device* device)
{
	return make_shared<DirectX11RenderTarget>(desc, device);
}

void DirectX11RenderTarget::CreateDepthStencilResource(RenderTargetDesc desc, ID3D11Device* device)
{
    if (desc.usage == RenderTargetUsage::Default)
    {
        // depth stencil buffer
        D3D11_TEXTURE2D_DESC descDepth{};
        descDepth.Width = desc.width;
        descDepth.Height = desc.height;
        descDepth.MipLevels = 1;
        descDepth.ArraySize = 1;
        // Most video cards come with a 32 bit depth buffer, it is then up to you how you want to use those 32 bits.
        // set the depth buffer format to DXGI_FORMAT_D24_UNORM_S8_UINT.
        // What this means is that we use the depth buffer as both a depth buffer and a stencil buffer,
        // and the format of the buffer is set to 24 bits for the depth channel and 8 bits for the stencil channel.
        // http://www.rastertek.com/dx11tut35.html
        descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDepth.SampleDesc.Count = 1;
        descDepth.SampleDesc.Quality = 0;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        descDepth.CPUAccessFlags = 0;
        descDepth.MiscFlags = 0;
        auto hr = device->CreateTexture2D( &descDepth, nullptr, &m_DepthStencilTexture);
        ASUNA_ASSERT(SUCCEEDED(hr));

        //  depth stencil view
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
        descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;
        hr = device->CreateDepthStencilView(m_DepthStencilTexture, &descDSV, &m_DepthStencilView);
        ASUNA_ASSERT(SUCCEEDED(hr));
    }
    else if (desc.usage == RenderTargetUsage::ShadowMap)
    {
        // depth stencil buffer
        D3D11_TEXTURE2D_DESC descDepth{};
        descDepth.Width = desc.width;
        descDepth.Height = desc.height;
        descDepth.MipLevels = 1;
        descDepth.ArraySize = 1;
        //https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-depth-stencil#reading-the-depth-stencil-buffer-as-a-texture
        descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
        descDepth.SampleDesc.Count = 1;
        descDepth.SampleDesc.Quality = 0;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        //https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-depth-stencil#reading-the-depth-stencil-buffer-as-a-texture
        descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
        descDepth.CPUAccessFlags = 0;
        descDepth.MiscFlags = 0;
        auto hr = device->CreateTexture2D( &descDepth, nullptr, &m_DepthStencilTexture);
        ASUNA_ASSERT(SUCCEEDED(hr));
        //  depth stencil view
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
        descDSV.Format = DXGI_FORMAT_D32_FLOAT;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;
        hr = device->CreateDepthStencilView(m_DepthStencilTexture, &descDSV, &m_DepthStencilView);
        ASUNA_ASSERT(SUCCEEDED(hr));
        // shader resource view
        D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
        // https://takinginitiative.wordpress.com/2011/05/15/directx10-tutorial-10-shadow-mapping/
        shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
        shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
        shaderResourceViewDesc.Texture2D.MipLevels = 1;
        hr = device->CreateShaderResourceView(m_DepthStencilTexture, &shaderResourceViewDesc, &m_DepthStencilSRV);
        ASUNA_ASSERT(SUCCEEDED(hr));
    }
    else
    {
        ASUNA_ASSERT(false);
    }
}

void DirectX11RenderTarget::Resize(int width, int height)
{
    auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Instance->GetContext());
    if (m_Desc.usage == RenderTargetUsage::Default)
    {
        RenderTarget::Resize(width, height);
        if (m_RenderTargetTexture == nullptr)
        {
            ClearRenderTarget();
            ClearDepthStencilResource();
            auto hr = context->m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
            ASUNA_ASSERT(SUCCEEDED(hr));
            ID3D11Texture2D* pBuffer;
            hr = context->m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
            ASUNA_ASSERT(SUCCEEDED(hr));
            hr = context->m_Device->CreateRenderTargetView(pBuffer, nullptr, &m_RenderTargetView);
            ASUNA_ASSERT(SUCCEEDED(hr));
            pBuffer->Release();
            m_Desc.width = width;
            m_Desc.height = height;
            CreateDepthStencilResource(m_Desc, context->m_Device);
        }
        else
        {
            ClearRenderTarget();
            ClearDepthStencilResource();
            m_Desc.width = width;
            m_Desc.height = height;
            CreateRenderTarget(m_Desc, context->m_Device);
            CreateDepthStencilResource(m_Desc, context->m_Device);
        }
    }
    else if (m_Desc.usage == RenderTargetUsage::ShadowMap)
    {
        ASUNA_ASSERT(false);
    }
}

void DirectX11RenderTarget::Bind()
{
    auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Instance->GetContext());
    ID3D11SamplerState* state = nullptr;
    if (m_Desc.usage == RenderTargetUsage::ShadowMap)
    {
        // shadow map bind slot 1 in our engine
        context->m_DeviceContext->PSSetShaderResources(1, 1, &m_DepthStencilSRV);
        context->m_DeviceContext->PSSetSamplers(1, 1, &state);
    }
    else
    {
        ASUNA_ASSERT(false);
    }
}











