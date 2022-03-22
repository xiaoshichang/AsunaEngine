#pragma once
#include "../Renderer.h"
#include "../../DLLExport/DLLExport.h"

namespace asuna
{

	class ASUNAENGINE_API DirectX12Renderer : public Renderer
	{
	public:
		virtual void Initialize();
		virtual void Finalize();
		virtual void Render();
		virtual void ClearRenderTarget(float r, float g, float b, float a) override;
		virtual void CreateDeviceContext() override;
		virtual void ReleaseDeviceContext() override;

	};
}


