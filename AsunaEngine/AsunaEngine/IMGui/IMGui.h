#include <Windows.h>
#include "../Foundation/Interface/IModule.h"


namespace asuna
{
	class IMGui
	{
	public:

		static void Initialize();
		static void Render();
		static void Finalize();
		static HRESULT HandleEvents(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		static void TestGUI();
	};

}