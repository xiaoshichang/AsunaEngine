
#pragma once
#include <iostream>
#include <memory>
#include <d3d11.h>
#include "../RenderTarget.h"

namespace asuna
{

	class DirectX11RenderTarget : public RenderTarget
	{
	public:
		DirectX11RenderTarget(ID3D11RenderTargetView* rtv, ID3D11Texture2D* tex) :
			m_RenderTargetView(rtv),
			m_RenderTargetTexture(tex)
		{
		}

		virtual ~DirectX11RenderTarget();

		ID3D11RenderTargetView* GetRenderTargetView()
		{
			return m_RenderTargetView;
		}

		void SetRenderTarget(ID3D11RenderTargetView* rtv)
		{
			m_RenderTargetView = rtv;
		}

	private:
		ID3D11Texture2D* m_RenderTargetTexture;
		ID3D11RenderTargetView* m_RenderTargetView;

	public:
		static std::shared_ptr<DirectX11RenderTarget> CreateFromSwapChain(ID3D11Device* device, IDXGISwapChain* swapChain);
		static std::shared_ptr<DirectX11RenderTarget> Create();
	};
}