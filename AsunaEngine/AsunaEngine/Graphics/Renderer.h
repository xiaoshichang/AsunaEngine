#pragma once
#include <Windows.h>
#include <memory>
#include <string>
#include "RenderAPIType.h"
#include "RenderContext.h"
#include "RenderItem.h"
#include "RenderItemQueue.h"
#include "RenderTarget.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include "Mesh.h"

#include "../Foundation/Interface/IModule.h"

namespace asuna
{
	struct CreateRendererContextParam
	{
		int m_ResolutionWith;
		int m_ResolutionHeight;
		HWND m_HWND;
	};


	class Renderer
	{
	public:
		Renderer() :
			m_Context(nullptr)
		{
			m_APIType = RenderAPIType::None;
		}

		virtual ~Renderer() = default;

		virtual void Initialize(CreateRendererContextParam param) = 0;
		virtual void Finalize() = 0;
		virtual void ResizeResolution(int width, int height) = 0;
		virtual void Present() = 0;
		virtual void ClearRenderTarget(std::shared_ptr<RenderTarget> rt, float r, float g, float b, float a) = 0;
		virtual void CreateDeviceContext() = 0;
		virtual void ReleaseDeviceContext() = 0;
		virtual void SetRenderTarget(std::shared_ptr<RenderTarget> rt) = 0;
		virtual std::shared_ptr<Mesh> CreateMesh(const std::string& path) = 0;
		virtual std::shared_ptr<RenderTarget> CreateRenderTarget(RenderTargetDesc desc) = 0;
		virtual std::shared_ptr<Shader> CreateShader(const std::string& path, ShaderType shaderType) = 0;

		virtual std::shared_ptr<RenderItem> CreateRenderItem(
                std::shared_ptr<Mesh> mesh,
                std::shared_ptr<Shader> vertexShader,
                std::shared_ptr<Shader> pixelShader,
                std::shared_ptr<ConstantBuffer> perObject,
                std::shared_ptr<ConstantBuffer> perScene) = 0;

		virtual std::shared_ptr<ConstantBuffer> CreateConstantBuffer(ConstantBufferDataType dt) = 0;
		std::shared_ptr<RenderContext> GetContext();

    public:
        bool CheckLeftHandRenderAPI() const;


	protected:
		std::shared_ptr<RenderContext> m_Context;

	public:
		RenderAPIType m_APIType;
		int m_ResolutionWidth = 0;
		int m_ResolutionHeight = 0;
		HWND m_HWND{};

	public:
		static Renderer* Current;
	};
}