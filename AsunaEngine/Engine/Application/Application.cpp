#include "Application.h"
#include "../AssetLoader/AssetLoader.h"
#include "../Foundation/Logger/Logger.h"

using namespace asuna;

Application::Application() : m_Quit(false)
{
}

Application::~Application()
{
	AssetLoader::Finalize();
}

void Application::Run()
{
	while (true)
	{
		Tick();
		if (m_Quit)
		{
			break;
		}
		Render();
	}
}

void Application::Initialize(std::shared_ptr<ApplicationInitParam> param)
{
    m_Config = param;
    Logger::InitLogger(param->LogDir, param->LogFile);
	AssetLoader::Init();
}
