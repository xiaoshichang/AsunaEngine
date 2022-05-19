#pragma once
#include <Windows.h>
#include <memory>
#include <string>
#include "RenderAPIType.h"
#include "RenderContext.h"
#include "RenderItem.h"
#include "RenderTarget.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "DepthStencilState.h"
#include "Texture.h"
#include "RasterizationState.h"

#include "../../Foundation/Interface/IModule.h"
#include "../../Foundation/Platform/Platform.h"

namespace asuna
{
	struct CreateRendererContextParam
	{
		int m_ResolutionWith;
		int m_ResolutionHeight;
#ifdef ASUNA_PLATFORM_WINDOWS
		HWND m_HWND;
#endif
	};


	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();
		std::shared_ptr<RenderContext> GetContext();
		RenderAPIType GetRenderAPIType() const;

	public:
		virtual void Initialize(CreateRendererContextParam param) = 0;
		virtual void Finalize() = 0;
		virtual void ResizeResolution(int width, int height) = 0;
        virtual void SetViewPort(int x, int y, int width, int height) = 0;
		virtual void Present() = 0;
		virtual void ClearRenderTarget(std::shared_ptr<RenderTarget> rt, float r, float g, float b, float a) = 0;
		virtual void CreateDeviceContext() = 0;
		virtual void ReleaseDeviceContext() = 0;
		virtual void SetRenderTarget(const std::shared_ptr<RenderTarget>& rt) = 0;
		virtual void SetRasterizationState(const std::shared_ptr<RasterizationState>& rs) = 0;
		virtual std::shared_ptr<Mesh> CreateMesh(const std::string& path) = 0;
        virtual std::shared_ptr<Mesh> CreateMesh(const std::shared_ptr<MeshCreateParam>& param) = 0;
		virtual std::shared_ptr<RenderTarget> CreateRenderTarget(RenderTargetDesc desc) = 0;
		virtual std::shared_ptr<Shader> CreateShader(const std::string& path, ShaderType shaderType) = 0;
        virtual std::shared_ptr<Material> CreateMaterial(const std::string& materialPath, MaterialType mt) = 0;
        virtual std::shared_ptr<DepthStencilState> CreateDepthStencilState() = 0;
		virtual std::shared_ptr<RenderItem> CreateRenderItem(const std::shared_ptr<Mesh>& mesh, const std::vector<std::shared_ptr<Material>>& materials, const std::shared_ptr<ConstantBuffer>& perObject) = 0;
        virtual std::shared_ptr<RenderItem> CreateRenderItem(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<ConstantBuffer>& perObject) = 0;
		virtual std::shared_ptr<ConstantBuffer> CreateConstantBuffer(ConstantBufferDataType dt, int size) = 0;
        virtual std::shared_ptr<Texture> CreateTexture(const std::string& path) = 0;
		virtual std::shared_ptr<RasterizationState> CreateRasterizationState(const RasterizationStateDesc& desc) = 0;

	protected:
		void CreateDefaultRasterizationState();

	protected:
		std::shared_ptr<RenderContext> m_Context = nullptr;
		std::shared_ptr<RasterizationState> m_DefaultRasterizationState = nullptr;

	public:
		RenderAPIType m_APIType;
		int m_ResolutionWidth = 0;
		int m_ResolutionHeight = 0;
#ifdef ASUNA_PLATFORM_WINDOWS
		HWND m_HWND{};
#endif

	public:
		static Renderer* Instance;
	};
}