#include "WindowsApplication.h"

#include "../../Foundation/Platform/Assert.h"
#include "../../Foundation/Math/AMath.h"
#include "../../Graphics/Renderer.h"

#include "../../Graphics/Empty/EmptyRenderer.h"
#include "../../Graphics/DirectX12/DirectX12Renderer.h"
#include "../../Graphics/DirectX11/DirectX11Renderer.h"
#include "../../Graphics/Opengl/OpenGLRenderer.h"
#include "../../IMGui/IMGui.h"
#include "../../AssetLoader/AssetLoader.h"

using namespace asuna;
using namespace std;

void WindowsApplication::Initialize()
{
	InitMainWindow();
	InitRenderer(RenderAPIType::Directx11);
	IMGui::Initialize();
}

void WindowsApplication::Finalize()
{
	IMGui::Finalize();
	Renderer::Current->Finalize();
}


void WindowsApplication::Tick()
{
	MSG msg = {};
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void WindowsApplication::Render()
{
	Renderer::Current->Render();
	IMGui::Render();
	Renderer::Current->Present();
}

HWND WindowsApplication::GetWindowsHandler() noexcept
{
	return m_HWND;
}

LRESULT WindowsApplication::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (IMGui::HandleEvents(hWnd, message, wParam, lParam))
	{
		return true;
	}

	auto app = reinterpret_cast<WindowsApplication*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	switch (message)
	{
		case WM_CREATE: {
			LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
			return 0;
		}
		case WM_DESTROY:
		{
			app->m_Quit = true;
			return 0;
		}
		case WM_SIZE:
		{
			if (Renderer::Current != nullptr)
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				Renderer::Current->ResizeResolution(width, height);
				return 0;
			}
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void WindowsApplication::InitMainWindow()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = "AsunaEngine";
	RegisterClassEx(&windowClass);

	RECT windowRect = { 0, 0, 1024, 768 };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window and store a handle to it.
	m_HWND = CreateWindowEx(
		NULL,
		windowClass.lpszClassName,
		"AsunaEngine",
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

void WindowsApplication::InitRenderer(RenderAPIType api)
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

	CreateRendererContextParam param;
	param.m_HWND = m_HWND;
	param.m_ResolutionWith = 1024;
	param.m_ResolutionHeight = 768;
	Renderer::Current->Initialize(param);
}



