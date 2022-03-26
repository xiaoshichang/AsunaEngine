#include "AsunaEngine/Foundation/Platform/Assert.h"
#include "WindowsPlayer.h"
#include "AsunaEngine/Foundation/Math/AMath.h"
#include "AsunaEngine/Graphics/Renderer.h"

#include "AsunaEngine/Graphics/Empty/EmptyRenderer.h"
#include "AsunaEngine/Graphics/DirectX12/DirectX12Renderer.h"
#include "AsunaEngine/Graphics/DirectX11/DirectX11Renderer.h"
#include "AsunaEngine/Graphics/Opengl/OpenGLRenderer.h"
#include "AsunaEngine/AssetLoader/AssetLoader.h"

using namespace asuna;
using namespace std;

void WindowsPlayer::Initialize()
{
	InitMainWindow();
	InitRenderer(RenderAPIType::Directx11);
}

void WindowsPlayer::Finialize()
{
	Renderer::Current->Finalize();
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
	Renderer::Current->Render();
}

HWND WindowsPlayer::GetWindowsHandler() noexcept
{
	return m_HWND;
}

LRESULT WindowsPlayer::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void WindowsPlayer::InitMainWindow()
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
	m_HWND = CreateWindowEx(
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
	ShowWindow(m_HWND, 5);
}

void WindowsPlayer::InitRenderer(RenderAPIType api)
{
	ASUNA_ASSERT(Renderer::Current == nullptr);
	if (api == RenderAPIType::None)
	{
		Renderer::Current = new EmptyRenderer();
	}
	if (api == RenderAPIType::Directx11)
	{
		Renderer::Current = new DirectX11Renderer();
	}
	else if (api == RenderAPIType::Directx12)
	{
		Renderer::Current = new DirectX12Renderer();
	}
	else if (api == RenderAPIType::Opengl)
	{
		Renderer::Current = new OpenGLRenderer();
	}
	else
	{
		ASUNA_ASSERT(false);
	}
	auto surface = make_shared<RenderSurfaceWindowsApplication>(m_HWND);
	Renderer::Current->SetRenderSurface(surface);
	Renderer::Current->Initialize();
}


