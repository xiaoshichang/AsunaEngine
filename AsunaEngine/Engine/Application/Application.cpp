#include "Application.h"
#include "../Foundation/Logger/Logger.h"


asuna::Application::Application() : m_Quit(false)
{
}

asuna::Application::~Application() = default;

void asuna::Application::Run()
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

void asuna::Application::Initialize(std::shared_ptr<ApplicationInitParam> param)
{
    Logger::InitLogger(param->LogDir, param->LogFile);
}
