
#include <memory>
#include "WindowsEditor.h"

using namespace std;
using namespace asuna;

int main()
{
	auto app = new WindowsEditor();
    auto param = make_shared<ApplicationInitParam>();
    param->RenderAPIType = RenderAPIType::Opengl;
    param->WindowWidth = 1920;
    param->WindowHeight = 1200;
    param->LogFile = (char*)"Editor";
    param->LogDir = (char*)"Logs";
    param->fromEditor = true;

	app->Initialize(param);
	app->Run();
	app->Finalize();
	exit(0);
}