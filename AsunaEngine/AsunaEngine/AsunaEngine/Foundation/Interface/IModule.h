#pragma once

namespace asuna
{

	class IModule
	{
	public:
		virtual void Initialize() = 0;
		virtual void Finalize() = 0;
	};

	class ITickableModule : IModule
	{
	public:
		virtual void Tick() = 0;
	};

}