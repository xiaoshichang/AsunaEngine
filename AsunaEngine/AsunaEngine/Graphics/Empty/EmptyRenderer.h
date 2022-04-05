#pragma once
#include "../Renderer.h"

namespace asuna
{

	class EmptyRenderer : public Renderer
	{
	public:
		 void Initialize(CreateRendererContextParam param) override;
		 void Finalize() override;
		 void ResizeResolution(int width, int height) override;
		 void ClearRenderTarget(std::shared_ptr<RenderTarget> rt, float r, float g, float b, float a) override;
		 void SetRenderTarget(std::shared_ptr<RenderTarget> rt) override;

		 std::shared_ptr<Mesh> CreateMesh(const std::string& scenePath) override;
		 std::shared_ptr<RenderTarget> CreateRenderTarget(RenderTargetDesc desc) override;
		 std::shared_ptr<Shader> CreateShader(const std::string& scenePath, ShaderType shaderType) override;
		 std::shared_ptr<RenderItem> CreateRenderItem(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> pixelShader, std::shared_ptr<ConstantBuffer> perObject) override;
		 std::shared_ptr<ConstantBuffer> CreateConstantBuffer(ConstantBufferDataType dt) override;
         std::shared_ptr<RenderItemQueue> CreateRenderItemQueue() override;
	protected:
		 void CreateDeviceContext() override;
		 void ReleaseDeviceContext() override;
		 void Present() override;


	};
}

