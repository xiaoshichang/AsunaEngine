#include <Windows.h>
#include "../Foundation/Interface/IModule.h"


namespace asuna
{
	class GUI
	{
	public:

		static void Initialize();
		static void Begin();
		static void End();
		static void Finalize();
		static HRESULT HandleEvents(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static void HandleMultiViewport(HWND hWnd, LPARAM lParam);
	private:
		static void TestGUI();
	};

}