

#include "iostream"
#include "WindowsEditor.h"

using namespace std;
using namespace asuna;

int main()
{
	auto app = new WindowsEditor();
	app->Initialize();
	app->Run();
	app->Finalize();
	exit(0);
}