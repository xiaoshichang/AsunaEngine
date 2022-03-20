
#include "iostream"
#include "../AsunaEnginePlayer/WindowsPlayer.h"

using namespace std;
using namespace Asuna;

int main()
{
	auto app = new WindowsPlayer();
	app->Initialize();
	app->Run();
	app->Finialize();
	exit(0);
}