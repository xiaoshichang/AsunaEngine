#pragma once
#include <d3d11.h>
#include <Windows.h>
#include "../Renderer.h"
#include "../../DLLExport/DLLExport.h"

namespace asuna
{
	class ASUNAENGINE_API DirectX11Renderer : public Renderer
	{
	public:
		DirectX11Renderer():
			m_swapChain(nullptr),
			m_RenderTargetView(nullptr),
			m_depthStencilBuffer(nullptr),
			m_depthStencilState(nullptr),
			m_depthStencilView(nullptr),
			m_rasterState(nullptr),
			m_videoCardMemory(0)
		{
		}

		virtual ~DirectX11Renderer()
		{
		}

	public:
		virtual void Initialize(CreateRendererContextParam param) override;
		virtual void Finalize() override;
		virtual void ResizeResolution(int width, int height) override;
		virtual void Present();

	protected:
		virtual void ClearRenderTarget(float r, float g, float b, float a) override;
		virtual void CreateDeviceContext() override;
		virtual void ReleaseDeviceContext() override;

	private:
		void InitTriangle();
		void SetViewPort(int width, int height);

	private:
		IDXGISwapChain* m_swapChain;
		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11Texture2D* m_depthStencilBuffer;
		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11RasterizerState* m_rasterState;
		int m_videoCardMemory;
		
	};
}

