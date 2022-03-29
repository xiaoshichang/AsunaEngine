

#include "iostream"
#include "WindowsEditor.h"

using namespace std;
using namespace asuna;

int main()
{
	auto app = new WindowsEditor();
    ApplicationInitParam param {0};
    param.RenderAPIType = RenderAPIType::Directx11;
    param.WindowWidth = 1200;
    param.WindowHeight = 800;

	app->Initialize(param);
	app->Run();
	app->Finalize();
	exit(0);
}