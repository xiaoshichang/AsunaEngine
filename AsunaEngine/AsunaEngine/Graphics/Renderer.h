#pragma once
#include <Windows.h>
#include <memory>
#include "../Foundation/Interface/IModule.h"
#include "RenderAPIType.h"
#include "RenderContext.h"
#include "RenderItem.h"
#include "RenderItemQueue.h"
#include "Shader.h"

#include "../DLLExport/DLLExport.h"

namespace asuna
{
	// explicit export template class
	template  class ASUNAENGINE_API std::shared_ptr<RenderContext>;
	template  class ASUNAENGINE_API std::shared_ptr<RenderItemQueue>;


	struct CreateRendererContextParam
	{
		int m_ResolutionWith;
		int m_ResolutionHeight;
		HWND m_HWND;
	};


	class ASUNAENGINE_API Renderer
	{
	public:
		Renderer() :
			m_Context(nullptr)
		{
			m_APIType = RenderAPIType::None;
			m_RenderItemQueue = std::make_shared<RenderItemQueue>();
		}

		virtual ~Renderer()
		{
		}

		virtual void Initialize(CreateRendererContextParam param) = 0;
		virtual void Finalize() = 0;
		virtual void Render();
		virtual void ResizeResolution(int width, int height) = 0;
		virtual void Present() = 0;

	private:
		virtual void ClearRenderTarget(float r, float g, float b, float a) = 0;
		virtual void CreateDeviceContext() = 0;
		virtual void ReleaseDeviceContext() = 0;


	public:
		std::shared_ptr<RenderContext> GetContext();
		virtual void AddRenderItem(std::shared_ptr<RenderItem> item);
		virtual void RemoveRenderItem(std::shared_ptr<RenderItem> item);


	protected:
		std::shared_ptr<RenderContext> m_Context;
		std::shared_ptr<RenderItemQueue> m_RenderItemQueue;

	public:
		RenderAPIType m_APIType;
		int m_ResolutionWidth = 0;
		int m_ResolutionHeight = 0;
		HWND m_HWND;

	public:
		static Renderer* Current;
	};
}