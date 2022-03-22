#pragma once
#include "../Renderer.h"

namespace asuna
{

	class ASUNAENGINE_API EmptyRenderer : public Renderer
	{
	public:
		virtual void Initialize();
		virtual void Finalize();
		virtual void Render();

	};
}

