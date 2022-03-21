#pragma once

namespace asuna
{

	class IModule
	{
		virtual void Initialize() = 0;
		virtual void Finalize() = 0;
	};

	class ITickableModule : IModule
	{
		virtual void Tick() = 0;
	};

}