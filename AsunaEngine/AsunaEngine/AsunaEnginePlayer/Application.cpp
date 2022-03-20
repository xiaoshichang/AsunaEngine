#include "Application.h"

void Asuna::Application::Run()
{
	while (true)
	{
		Tick();
		Render();
	}
}
