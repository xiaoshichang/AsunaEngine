#pragma once
#include <memory>
#include "../Foundation/Interface/IModule.h"
#include "RenderAPIType.h"
#include "RenderSurface.h"
#include "RenderContext.h"
#include "RenderItem.h"
#include "RenderItemQueue.h"
#include "Shader.h"

#include "../DLLExport/DLLExport.h"

namespace asuna
{
	// explicit export template class
	template  class __declspec(dllexport) std::shared_ptr<RenderSurface>;
	template  class __declspec(dllexport) std::shared_ptr<RenderContext>;
	template  class __declspec(dllexport) std::shared_ptr<RenderItemQueue>;


	class ASUNAENGINE_API Renderer : public IModule
	{
	public:
		Renderer() :
			m_Surface(nullptr),
			m_Context(nullptr)
		{
			m_APIType = RenderAPIType::None;
			m_RenderItemQueue = std::make_shared<RenderItemQueue>();
		}

		virtual ~Renderer()
		{
		}

		virtual void Render();
		virtual void SetRenderSurface(std::shared_ptr<RenderSurface> surface);
		virtual std::shared_ptr<RenderContext> GetContext();
		virtual void AddRenderItem(std::shared_ptr<RenderItem> item);
		virtual void RemoveRenderItem(std::shared_ptr<RenderItem> item);
		RenderAPIType GetRenderAPIType();

	

	protected:
		virtual void ClearRenderTarget(float r, float g, float b, float a) = 0;
		virtual void Present() = 0;
		virtual void CreateDeviceContext() = 0;
		virtual void ReleaseDeviceContext() = 0;

	protected:
		RenderAPIType m_APIType;
		std::shared_ptr<RenderSurface> m_Surface;
		std::shared_ptr<RenderContext> m_Context;
		std::shared_ptr<RenderItemQueue> m_RenderItemQueue;

	public:
		static Renderer* Current;
	};
}