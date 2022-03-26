#include "Application.h"

void asuna::Application::Run()
{
	while (!m_Quit)
	{
		Tick();
		Render();
	}
}
