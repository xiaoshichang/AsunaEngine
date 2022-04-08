#pragma once
#include <memory>
#include "../Graphics/Abstract/RenderAPIType.h"

namespace asuna
{
    struct ApplicationInitParam
    {
        int WindowWidth;
        int WindowHeight;
        RenderAPIType RenderAPIType;
        char* LogDir;
        char* LogFile;
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
		virtual void Initialize(std::shared_ptr<ApplicationInitParam> param);
		virtual void Finalize() = 0;
		virtual void Run();
		virtual void Tick() = 0;
		virtual void Render() = 0;

	public:
		bool m_Quit;
	};
}


