
#include "iostream"
#include "Player/Windows/WindowsPlayer.h"

using namespace std;
using namespace asuna;

int main()
{
    auto param = make_shared<ApplicationInitParam>();
    param->RenderAPIType = RenderAPIType::Directx11;
    param->WindowWidth = 1200;
    param->WindowHeight = 800;
    param->LogFile = (char*)"Player";
    param->LogDir = (char*)"Logs";
    param->fromEditor = false;

	auto app = new WindowsPlayer();
	app->Initialize(param);
	app->Run();
	app->Finalize();
	exit(0);
}