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
}

std::shared_ptr<DirectX11RenderTarget> asuna::DirectX11RenderTarget::CreateFromSwapChain(ID3D11Device* device, IDXGISwapChain* swapChain)
{
	ID3D11RenderTargetView* rtv;

	// Get the pointer to the back buffer.
	ID3D11Texture2D* backBufferPtr;
	auto result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	ASUNA_ASSERT(result >= 0);

	// Create the render target view with the back buffer pointer.
	result = device->CreateRenderTargetView(backBufferPtr, NULL, &rtv);
	ASUNA_ASSERT(result >= 0);

	auto rt = std::make_shared<DirectX11RenderTarget>(rtv, nullptr);
	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;
	return rt;
}

std::shared_ptr<DirectX11RenderTarget> asuna::DirectX11RenderTarget::Create()
{
	return nullptr;
}
