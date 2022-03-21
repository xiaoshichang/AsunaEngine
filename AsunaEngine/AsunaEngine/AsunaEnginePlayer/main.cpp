
#include "iostream"
#include "Player/Windows/WindowsPlayer.h"

using namespace std;
using namespace asuna;

int main()
{
	auto app = new WindowsPlayer();
	app->Initialize();
	app->Run();
	app->Finialize();
	exit(0);
}