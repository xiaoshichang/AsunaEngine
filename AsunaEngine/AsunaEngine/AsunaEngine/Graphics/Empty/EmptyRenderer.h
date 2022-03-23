#pragma once
#include "../Renderer.h"

namespace asuna
{

	class ASUNAENGINE_API EmptyRenderer : public Renderer
	{
	public:
		virtual void Initialize();
		virtual void Finalize();
	protected:
		virtual void ClearRenderTarget(float r, float g, float b, float a) override;
		virtual void CreateDeviceContext() override;
		virtual void ReleaseDeviceContext() override;
		virtual void Present() override;
	};
}

