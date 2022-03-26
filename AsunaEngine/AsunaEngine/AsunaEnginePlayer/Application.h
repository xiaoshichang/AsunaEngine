#pragma once

namespace asuna
{
	class Application
	{
	public:
		Application() :
			m_Quit(false)
		{
		}

		virtual ~Application()
		{
		}

	public:
		virtual void Initialize() = 0;
		virtual void Finialize() = 0;
		virtual void Run();
		virtual void Tick() = 0;
		virtual void Render() = 0;

	public:
		bool m_Quit;
	};
}


