#pragma once

namespace Asuna
{
	class Application
	{
	public:
		virtual void Initialize() = 0;
		virtual void Finialize() = 0;
		virtual void Run();
		virtual void Tick() = 0;
		virtual void Render() = 0;
	};
}


