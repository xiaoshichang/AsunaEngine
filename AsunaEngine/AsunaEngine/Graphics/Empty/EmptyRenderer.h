#pragma once
#include "../Renderer.h"

namespace asuna
{

	class ASUNAENGINE_API EmptyRenderer : public Renderer
	{
	public:
		virtual void Initialize(CreateRendererContextParam param);
		virtual void Finalize();
		virtual void ResizeResolution(int width, int height);
		virtual void ClearRenderTarget(std::shared_ptr<RenderTarget> rt, float r, float g, float b, float a) override;
		virtual void SetRenderTarget(std::shared_ptr<RenderTarget> rt) override;

		virtual std::shared_ptr<Mesh> CreateMesh(const std::string& scenePath) override;
		virtual std::shared_ptr<RenderTarget> CreateRenderTarget() override;
		virtual std::shared_ptr<Shader> CreateShader(const std::string& scenePath, ShaderType shaderType) override;
		virtual std::shared_ptr<RenderItem> CreateRenderItem(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> pixelShader, std::shared_ptr<ConstantBuffer> constantBuffer) override;
		virtual std::shared_ptr<ConstantBuffer> CreateConstantBuffer() override;
	protected:
		virtual void CreateDeviceContext() override;
		virtual void ReleaseDeviceContext() override;
		virtual void Present() override;

		

		// ͨ�� Renderer �̳�
		

	};
}

