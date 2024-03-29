#include <iostream>
#include "Player/Windows/WindowsPlayer.h"

#include "Engine/Graphics/Abstract/Material.h"

using namespace std;
using namespace asuna;

int main()
{
	//_CrtSetBreakAlloc(2508);

    auto param = make_shared<ApplicationInitParam>();
    param->RenderAPIType = RenderAPIType::Directx11;
    param->WindowWidth = 1200;
    param->WindowHeight = 800;
    param->LogFile = (char*)"Player";
    param->LogDir = (char*)"Logs";
    param->EditorMode = false;

	auto app = new WindowsPlayer();
	app->Initialize(param);
	app->Run();
	app->Finalize();
    delete app;

    return 0;
}