#pragma once
#include <memory>
#include <d3d11.h>
#include "DirectX11RenderTarget.h"
#include "../Abstract/RenderContext.h"
namespace asuna
{
	class DirectX11RenderContext : public RenderContext
	{
	public:
		DirectX11RenderContext() = delete;
		DirectX11RenderContext(
			ID3D11Device* device, 
			ID3D11DeviceContext* deviceContext, 
			IDXGISwapChain* swapChain,
			std::shared_ptr<DirectX11RenderTarget> mainrt
		):
			m_Device(device),
			m_DeviceContext(deviceContext),
			m_swapChain(swapChain),
			m_MainRT(mainrt)
		{
		}

		virtual ~DirectX11RenderContext()
		{
			if (m_swapChain != nullptr)
			{
				m_swapChain->Release();
				m_swapChain = nullptr;
			}
			if (m_DeviceContext != nullptr)
			{
				m_DeviceContext->Release();
				m_DeviceContext = nullptr;
			}
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}

	public:
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
		IDXGISwapChain* m_swapChain;
		std::shared_ptr<DirectX11RenderTarget> m_MainRT;

	};
}


