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
		virtual void Initialize() override;
		virtual void Finalize() override;

	protected:
		virtual void ClearRenderTarget(float r, float g, float b, float a) override;
		virtual void Present();
		virtual void CreateDeviceContext() override;
		virtual void ReleaseDeviceContext() override;

	private:
		void InitTriangle();

	private:
		HWND m_hwnd;
		char m_videoCardDescription[128];
		IDXGISwapChain* m_swapChain;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11Texture2D* m_depthStencilBuffer;
		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11RasterizerState* m_rasterState;
		int m_videoCardMemory;
		
	};
}

