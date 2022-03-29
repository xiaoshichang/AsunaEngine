#include "DirectX11RenderTarget.h"
#include "../../Foundation/Platform/Assert.h"
using namespace asuna;

asuna::DirectX11RenderTarget::~DirectX11RenderTarget()
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

std::shared_ptr<DirectX11RenderTarget> asuna::DirectX11RenderTarget::CreateFromSwapChain(RenderTargetDesc desc, ID3D11Device* device, IDXGISwapChain* swapChain)
{
	ID3D11RenderTargetView* rtv;

	// Get the pointer to the back buffer.
	ID3D11Texture2D* backBufferPtr;
	auto result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	ASUNA_ASSERT(result >= 0);

	// Create the render target view with the back buffer pointer.
	result = device->CreateRenderTargetView(backBufferPtr, NULL, &rtv);
	ASUNA_ASSERT(result >= 0);

	auto rt = std::make_shared<DirectX11RenderTarget>(desc, rtv, nullptr, nullptr);
	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;
	return rt;
}

std::shared_ptr<DirectX11RenderTarget> asuna::DirectX11RenderTarget::Create(RenderTargetDesc desc, ID3D11Device* device)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

    ID3D11Texture2D* texture;
    ID3D11RenderTargetView* rtv;
    ID3D11ShaderResourceView* srv;

    // Initialize the render target texture description.
    ZeroMemory(&textureDesc, sizeof(textureDesc));

    // Setup the render target texture description.
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
    auto result = device->CreateTexture2D(&textureDesc, NULL, &texture);
    ASUNA_ASSERT(SUCCEEDED(result));

    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;
    // Create the render target view.
    result = device->CreateRenderTargetView(texture, &renderTargetViewDesc, &rtv);
    ASUNA_ASSERT(SUCCEEDED(result));

    // Setup the description of the shader resource view.
    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;
    result = device->CreateShaderResourceView(texture, &shaderResourceViewDesc, &srv);

	return std::make_shared<DirectX11RenderTarget>(desc, rtv, texture, srv);
}
