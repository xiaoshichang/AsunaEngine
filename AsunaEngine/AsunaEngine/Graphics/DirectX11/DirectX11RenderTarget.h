
#pragma once
#include <iostream>
#include <memory>
#include <d3d11.h>
#include "../Abstract/RenderTarget.h"

namespace asuna
{

	class DirectX11RenderTarget : public RenderTarget
	{
	public:
		DirectX11RenderTarget(
                RenderTargetDesc desc,
                ID3D11RenderTargetView* rtv,
                ID3D11Texture2D* tex,
                ID3D11ShaderResourceView* srv) :
			m_RenderTargetView(rtv),
			m_RenderTargetTexture(tex),
            m_ShaderResourceView(srv),
            RenderTarget(desc)
		{
		}

		virtual ~DirectX11RenderTarget();

		ID3D11RenderTargetView* GetRenderTargetView()
		{
			return m_RenderTargetView;
		}

        ID3D11ShaderResourceView* GetSRV()
        {
            return m_ShaderResourceView;
        }

		void SetRenderTarget(ID3D11RenderTargetView* rtv)
		{
			m_RenderTargetView = rtv;
		}

	private:
		ID3D11Texture2D* m_RenderTargetTexture;
		ID3D11RenderTargetView* m_RenderTargetView;
        ID3D11ShaderResourceView* m_ShaderResourceView;

	public:
		static std::shared_ptr<DirectX11RenderTarget> CreateFromSwapChain(RenderTargetDesc desc, ID3D11Device* device, IDXGISwapChain* swapChain);
		static std::shared_ptr<DirectX11RenderTarget> Create(RenderTargetDesc desc, ID3D11Device* device);
	};
}