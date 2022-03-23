#pragma once
#include "../Foundation/Interface/IModule.h"
#include "RenderSurface.h"
#include "RenderContext.h"
#include "RenderItem.h"
#include "RenderItemQueue.h"
#include "Shader.h"

#include "../DLLExport/DLLExport.h"

namespace asuna
{

	class ASUNAENGINE_API Renderer : public IModule
	{
	public:
		Renderer()
		{
			m_Surface = 0;
			m_Context = 0;
			m_RenderItemQueue = new RenderItemQueue();
		}
		~Renderer()
		{
			delete m_RenderItemQueue;
		}

		virtual void Render();
		virtual void SetRenderSurface(RenderSurface* surface);
		virtual RenderContext* GetContext();
		virtual void AddRenderItem(RenderItem* item);
		virtual void RemoveRenderItem(RenderItem* item);

	protected:
		virtual void ClearRenderTarget(float r, float g, float b, float a) = 0;
		virtual void Present() = 0;
		virtual void CreateDeviceContext() = 0;
		virtual void ReleaseDeviceContext() = 0;

	protected:
		RenderSurface* m_Surface;
		RenderContext* m_Context;
		RenderItemQueue* m_RenderItemQueue;

	public:
		static Renderer* Current;
	};
}