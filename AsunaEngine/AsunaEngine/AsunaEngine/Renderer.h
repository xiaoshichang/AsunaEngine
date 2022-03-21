#pragma once

#include "IModule.h"

namespace asuna
{
	class Renderer : public ITickableModule
	{
	public:
		virtual void Render() = 0;
	};
}