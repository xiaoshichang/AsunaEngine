#pragma once
#include "../Graphics/RenderAPIType.h"

namespace asuna
{
    struct ApplicationInitParam
    {
        int WindowWidth;
        int WindowHeight;
        RenderAPIType RenderAPIType;
    };


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
		virtual void Initialize(ApplicationInitParam param) = 0;
		virtual void Finalize() = 0;
		virtual void Run();
		virtual void Tick() = 0;
		virtual void Render() = 0;

	public:
		bool m_Quit;
	};
}


