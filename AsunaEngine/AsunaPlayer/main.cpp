
#include "iostream"
#include "Player/Windows/WindowsPlayer.h"

using namespace std;
using namespace asuna;

int main()
{
    ApplicationInitParam param {0};
    param.RenderAPIType = RenderAPIType::Directx11;
    param.WindowWidth = 800;
    param.WindowHeight = 600;

	auto app = new WindowsPlayer();
	app->Initialize(param);
	app->Run();
	app->Finalize();
	exit(0);
}