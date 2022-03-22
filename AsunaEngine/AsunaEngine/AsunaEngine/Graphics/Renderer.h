#pragma once

#include "../Foundation/Interface/IModule.h"
#include "RenderSurface.h"
#include "../DLLExport/DLLExport.h"

namespace asuna
{
	class ASUNAENGINE_API Renderer : public IModule
	{
	public:
		virtual void Render() = 0;
		virtual void Initialize() = 0;
		virtual void SetRenderSurface(RenderSurface* surface);
		virtual void ClearRenderTarget(float r, float g, float b, float a) = 0;
		virtual void CreateDeviceContext() = 0;
		virtual void ReleaseDeviceContext() = 0;

	protected:
		RenderSurface* m_Surface;

	public:
		static Renderer* Current;
	};
}