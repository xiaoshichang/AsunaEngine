#pragma once
#include <memory>
#include "../Foundation/Platform/Platform.h"
#ifdef ASUNA_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace asuna
{
	class GUI
	{
	public:

		static void Initialize(bool docking = false, bool multivp = false);
		static void Begin();
		static void End();
		static void Finalize();

#ifdef ASUNA_PLATFORM_WINDOWS
		static HRESULT HandleEvents(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static void HandleMultiViewport(HWND hWnd, LPARAM lParam);
#endif

	private:
		static void TestGUI();
	};

}