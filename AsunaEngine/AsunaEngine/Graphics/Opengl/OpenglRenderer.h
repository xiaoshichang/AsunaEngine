#pragma once

#include "../Abstract/Renderer.h"

namespace asuna
{
	class OpenglRenderer : public Renderer
	{
	public:
		OpenglRenderer() = default;
		 ~OpenglRenderer() override = default;

	public:
		 void Initialize(CreateRendererContextParam param) override;
		 void Finalize() override;
		 void ResizeResolution(int width, int height) override;
		 void Present() override;
		 void SetRenderTarget(std::shared_ptr<RenderTarget> rt) override;
         void SetViewPort(int x, int y, int width, int height) override;

		 std::shared_ptr<Mesh> CreateMesh(const std::string& scenePath) override;
         std::shared_ptr<Mesh> CreateMesh(const std::shared_ptr<MeshCreateParam>& param) override;
		 std::shared_ptr<RenderTarget> CreateRenderTarget(RenderTargetDesc desc) override;
		 std::shared_ptr<Shader> CreateShader(const std::string& scenePath, ShaderType shaderType) override;
		 std::shared_ptr<RenderItem> CreateRenderItem(const std::shared_ptr<Mesh>& mesh, const std::vector<std::shared_ptr<Material>>& materials, const std::shared_ptr<ConstantBuffer>& perObject) override;
         std::shared_ptr<RenderItem> CreateRenderItem(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<ConstantBuffer>& perObject) override;
		 std::shared_ptr<ConstantBuffer> CreateConstantBuffer(ConstantBufferDataType dt, int size) override;
         std::shared_ptr<Material> CreateMaterial(const std::string& materialPath) override;
         std::shared_ptr<DepthStencilState> CreateDepthStencilState() override;
         std::shared_ptr<RenderItemQueue> CreateRenderItemQueue() override;

    public:
        void MakeCurrentContext();
        void SetConstantBufferDataPerFrame(ConstantBufferDataPerFrame* data) {m_ConstantBufferDataPerFrame = data;}
        void SetConstantBufferDataPerObject(ConstantBufferDataPerObject* data) {m_ConstantBufferDataPerObject = data;}
        ConstantBufferDataPerFrame* GetConstantBufferDataPerFrame() {return m_ConstantBufferDataPerFrame;}
        ConstantBufferDataPerObject* GetConstantBufferDataPerObject() {return m_ConstantBufferDataPerObject;}

	private:
		 void ClearRenderTarget(std::shared_ptr<RenderTarget> rt, float r, float g, float b, float a) override;
		 void CreateDeviceContext() override;
		 void ReleaseDeviceContext() override;

	private:
		void LoadWGL();
		void CreateGLContext();

	private:
		HGLRC m_RenderContext = nullptr;
		HDC   m_hDC = nullptr;
        ConstantBufferDataPerFrame* m_ConstantBufferDataPerFrame = nullptr;
        ConstantBufferDataPerObject* m_ConstantBufferDataPerObject = nullptr;

		

	};

}


