#pragma once
#include <d3d11.h>
#include <Windows.h>
#include "../Renderer.h"

namespace asuna
{
	class DirectX11Renderer : public Renderer
	{
	public:
		DirectX11Renderer():
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

		virtual void CreateDeviceContext() override;
		virtual void ReleaseDeviceContext() override;

	public:
		virtual void Initialize(CreateRendererContextParam param) override;
		virtual void Finalize() override;
		virtual void ResizeResolution(int width, int height) override;
		virtual void Present();
		virtual void ClearRenderTarget(std::shared_ptr<RenderTarget> rt, float r, float g, float b, float a) override;
		virtual void SetRenderTarget(std::shared_ptr<RenderTarget> rt) override;

		virtual std::shared_ptr<Mesh> CreateMesh(const std::string& scenePath) override;
		virtual std::shared_ptr<RenderTarget> CreateRenderTarget(RenderTargetDesc desc) override;
		virtual std::shared_ptr<Shader> CreateShader(const std::string& scenePath, ShaderType shaderType) override;
		virtual std::shared_ptr<ConstantBuffer> CreateConstantBuffer() override;
		virtual std::shared_ptr<RenderItem> CreateRenderItem(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> pixelShader, std::shared_ptr<ConstantBuffer> constantBuffer) override;

	private:
		void SetViewPort(ID3D11DeviceContext* deviceContext, int width, int height);
		void SetRasterizerState(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		void CreateDepthStencilState(ID3D11Device* device, ID3D11DeviceContext* deviceContext);


	private:


		ID3D11Texture2D* m_depthStencilBuffer;
		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11RasterizerState* m_rasterState;
		int m_videoCardMemory;

	};
}

