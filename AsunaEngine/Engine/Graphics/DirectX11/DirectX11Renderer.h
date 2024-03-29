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
		DirectX11Renderer()
		{
		}

		~DirectX11Renderer() override = default;

		void CreateDeviceContext() override;
		void ReleaseDeviceContext() override;

	public:
		void Initialize(CreateRendererContextParam param) override;
		void Finalize() override;
		void ResizeResolution(int width, int height) override;
		void Present() override;
		void ClearRenderTarget(shared_ptr<RenderTarget> rt, float r, float g, float b, float a) override;
		void SetRenderTarget(const shared_ptr<RenderTarget>& rt) override;
		void SetRasterizationState(const std::shared_ptr<RasterizationState>& rs) override;
        void SetViewPort(int x, int y, int width, int height) override;

		shared_ptr<Mesh> CreateMesh(const string& scenePath) override;
        shared_ptr<Mesh> CreateMesh(const std::shared_ptr<MeshCreateParam>& param) override;
		shared_ptr<RenderTarget> CreateRenderTarget(RenderTargetDesc desc) override;
		shared_ptr<Shader> CreateShader(const string& scenePath, ShaderType shaderType) override;
		shared_ptr<ConstantBuffer> CreateConstantBuffer(ConstantBufferDataType dt, int size) override;
        shared_ptr<Material> CreateMaterial(const std::string& materialPath, MaterialType mt) override;
        shared_ptr<DepthStencilState> CreateDepthStencilState() override;
		shared_ptr<RenderItem> CreateRenderItem(const std::shared_ptr<Mesh>& mesh, const vector<std::shared_ptr<Material>>& materials, const std::shared_ptr<ConstantBuffer>& perObject) override;
        shared_ptr<RenderItem> CreateRenderItem(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<ConstantBuffer>& perObject) override;
        shared_ptr<Texture> CreateTexture(const std::string& path) override;
		shared_ptr<RasterizationState> CreateRasterizationState(const RasterizationStateDesc& desc) override;

	};
}

