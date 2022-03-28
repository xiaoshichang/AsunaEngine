#include "Application.h"

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
