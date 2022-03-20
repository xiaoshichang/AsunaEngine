#pragma once

#include "IModule.h"

namespace Asuna
{
	class Renderer : public ITickableModule
	{
	public:
		virtual void Render() = 0;
	};
}