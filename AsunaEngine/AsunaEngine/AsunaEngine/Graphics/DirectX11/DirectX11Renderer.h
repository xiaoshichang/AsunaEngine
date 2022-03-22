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
		virtual void Initialize();
		virtual void Finalize();
		virtual void Render();

	private:
		void CreateDeviceContext();
		void ReleaseDeviceContext();
		void ClearRenderTarget(float r, float g, float b, float a);

		// test
		void InitPipeline();
		void InitTriangle();

	private:
		HWND m_hwnd;

		char m_videoCardDescription[128];
		IDXGISwapChain* m_swapChain;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11Texture2D* m_depthStencilBuffer;
		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11RasterizerState* m_rasterState;
		int m_videoCardMemory;


		// test
		ID3D11InputLayout* g_pLayout = nullptr;              // the pointer to the input layout
		ID3D11VertexShader* g_pVS = nullptr;              // the pointer to the vertex shader
		ID3D11PixelShader* g_pPS = nullptr;              // the pointer to the pixel shader
		ID3D11Buffer* g_pVBuffer = nullptr;
	};
}

