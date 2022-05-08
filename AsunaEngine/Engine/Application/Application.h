#pragma once
#include <memory>
#include "../Graphics/Abstract/RenderAPIType.h"

namespace asuna
{
    struct ApplicationInitParam
    {
        bool EditorMode;
        int WindowWidth;
        int WindowHeight;
        RenderAPIType RenderAPIType;
        char* LogDir;
        char* LogFile;
    };


	class Application
	{
	public:
		Application();
		virtual ~Application();


	public:
		virtual void Initialize(std::shared_ptr<ApplicationInitParam> param);
		virtual void Finalize() = 0;
		virtual void Run();
		virtual void Tick() = 0;
		virtual void Render() = 0;
        const std::shared_ptr<ApplicationInitParam>& GetConfig() const {return m_Config;}

	public:
		bool m_Quit;
        std::shared_ptr<ApplicationInitParam> m_Config;
	};
}


