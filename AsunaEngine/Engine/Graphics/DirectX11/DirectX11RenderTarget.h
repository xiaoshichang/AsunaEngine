
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
		explicit DirectX11RenderTarget(RenderTargetDesc desc, ID3D11Device* device);
        explicit DirectX11RenderTarget(RenderTargetDesc desc, ID3D11Device* device, IDXGISwapChain* swapChain);
		~DirectX11RenderTarget() override;
        void Resize(int width, int height) override;

        ID3D11RenderTargetView* GetRenderTargetView(){return m_RenderTargetView;}
        ID3D11DepthStencilView* GetDepthStencilView(){return m_DepthStencilView;}
        ID3D11ShaderResourceView* GetSRV(){return m_ShaderResourceView;}
    private:
        void ClearRenderTarget();
        void ClearDepthStencilResource();

        void CreateRenderTarget(RenderTargetDesc desc, ID3D11Device* device);
        void CreateRenderTargetFromSwapChain(RenderTargetDesc desc, ID3D11Device* device, IDXGISwapChain* swapChain);
        void CreateDepthStencilResource(RenderTargetDesc desc, ID3D11Device* device);


	private:
		ID3D11Texture2D* m_RenderTargetTexture = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
        ID3D11ShaderResourceView* m_ShaderResourceView = nullptr;
        ID3D11Texture2D* m_DepthStencilTexture = nullptr;
        ID3D11DepthStencilView* m_DepthStencilView = nullptr;


	public:
		static std::shared_ptr<DirectX11RenderTarget> CreateFromSwapChain(RenderTargetDesc desc, ID3D11Device* device, IDXGISwapChain* swapChain);
		static std::shared_ptr<DirectX11RenderTarget> Create(RenderTargetDesc desc, ID3D11Device* device);
	};
}