#pragma once

#include "../Foundation/Interface/IModule.h"

namespace asuna
{
	class Renderer : public ITickableModule
	{
	public:
		virtual void Render() = 0;
	};
}