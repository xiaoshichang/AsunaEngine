#pragma once
#include <d3d11.h>
#include <Windows.h>
#include "../Abstract/Renderer.h"

using namespace std;

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

		~DirectX11Renderer()
		{
		}

		void CreateDeviceContext() override;
		void ReleaseDeviceContext() override;

	public:
		void Initialize(CreateRendererContextParam param) override;
		void Finalize() override;
		void ResizeResolution(int width, int height) override;
		void Present();
		void ClearRenderTarget(shared_ptr<RenderTarget> rt, float r, float g, float b, float a) override;
		void SetRenderTarget(shared_ptr<RenderTarget> rt) override;
        void SetViewPort(int x, int y, int width, int height) override;

		shared_ptr<Mesh> CreateMesh(const string& scenePath) override;
		shared_ptr<RenderTarget> CreateRenderTarget(RenderTargetDesc desc) override;
		shared_ptr<Shader> CreateShader(const string& scenePath, ShaderType shaderType) override;
		shared_ptr<ConstantBuffer> CreateConstantBuffer(ConstantBufferDataType dt) override;
		shared_ptr<RenderItem> CreateRenderItem(const std::shared_ptr<Mesh>& mesh, const vector<std::shared_ptr<Material>>& materials, const std::shared_ptr<ConstantBuffer>& perObject) override;
        shared_ptr<RenderItemQueue> CreateRenderItemQueue() override;

	private:
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

