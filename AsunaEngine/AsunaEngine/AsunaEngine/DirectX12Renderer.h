#pragma once
#include "Renderer.h"
#include "DLLExport.h"

namespace Asuna
{

	class ASUNAENGINE_API DirectX12Renderer : public Renderer
	{
	public:
		virtual void Initialize();
		virtual void Finalize();
		virtual void Tick();
		virtual void Render();

	};
}


