#include "Application.h"

void asuna::Application::Run()
{
	while (true)
	{
		Tick();
		Render();
	}
}
