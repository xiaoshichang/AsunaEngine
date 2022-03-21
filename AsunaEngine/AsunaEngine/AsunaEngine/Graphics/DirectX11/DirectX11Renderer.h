#pragma once
#include "../Renderer.h"
#include "../../DLLExport/DLLExport.h"

namespace asuna
{
	class ASUNAENGINE_API DirectX11Renderer : public Renderer
	{
	public:
		virtual void Initialize();
		virtual void Finalize();
		virtual void Tick();
		virtual void Render();

	};
}

