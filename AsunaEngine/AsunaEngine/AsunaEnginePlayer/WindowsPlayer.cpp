#include "../AsunaEngine/Assert.h"
#include "WindowsPlayer.h"

using namespace asuna;


void WindowsPlayer::Initialize()
{
	CreateMainWindow();
}

void WindowsPlayer::Finialize()
{
}


void WindowsPlayer::Tick()
{
	MSG msg = {};
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void WindowsPlayer::Render()
{
}

HWND WindowsPlayer::GetWindowsHandler() noexcept
{
	return HWND_;
}

LRESULT WindowsPlayer::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void WindowsPlayer::CreateMainWindow()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = L"AsunaEngine";
	RegisterClassEx(&windowClass);

	RECT windowRect = { 0, 0, 1024, 768 };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window and store a handle to it.
	HWND_ = CreateWindowEx(
		NULL,
		windowClass.lpszClassName,
		L"AsunaEngine",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,        // We have no parent window.
		nullptr,        // We aren't using menus.
		hInstance,
		this);

	// Initialize the sample. OnInit is defined in each child-implementation of DXSample.
	ShowWindow(HWND_, 5);
}
