#pragma once

#include "../Renderer.h"

namespace asuna
{
	class OpenGLRenderer : public Renderer
	{
	public:
		OpenGLRenderer() = default;
		 ~OpenGLRenderer() override = default;

	public:
		 void Initialize(CreateRendererContextParam param) override;
		 void Finalize() override;
		 void ResizeResolution(int width, int height) override;
		 void Present() override;
		 void SetRenderTarget(std::shared_ptr<RenderTarget> rt) override;
         void SetViewPort(int x, int y, int width, int height) override;

		 std::shared_ptr<Mesh> CreateMesh(const std::string& scenePath) override;
		 std::shared_ptr<RenderTarget> CreateRenderTarget(RenderTargetDesc desc) override;
		 std::shared_ptr<Shader> CreateShader(const std::string& scenePath, ShaderType shaderType) override;
		 std::shared_ptr<RenderItem> CreateRenderItem(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> pixelShader, std::shared_ptr<ConstantBuffer> perObject) override;
		 std::shared_ptr<ConstantBuffer> CreateConstantBuffer(ConstantBufferDataType dt) override;
         std::shared_ptr<RenderItemQueue> CreateRenderItemQueue() override;

	private:
		 void ClearRenderTarget(std::shared_ptr<RenderTarget> rt, float r, float g, float b, float a) override;
		 void CreateDeviceContext() override;
		 void ReleaseDeviceContext() override;

	private:
		void LoadWGL();
		void CreateGLContext();
		void SetViewPort(int width, int height);


	private:
		HGLRC m_RenderContext = 0;
		HDC   m_hDC = 0;

		unsigned int g_vertexShader = 0;
		unsigned int g_fragmentShader = 0;
		unsigned int g_shaderProgram = 0;

		

	};

}


