#include "WindowsApplication.h"

#ifdef ASUNA_PLATFORM_WINDOWS
#include "../../Foundation/Platform/Assert.h"
#include "../../Foundation/Math/AMath.h"
#include "../../Graphics/Abstract/Renderer.h"
#include "../../Graphics/DirectX11/DirectX11Renderer.h"
#include "../../Graphics/Opengl/OpenGLRenderer.h"
#include "../../GUI/GUI.h"
#include "../../Scene/SceneManager.h"
#include "../../AssetLoader/AssetLoader.h"


using namespace asuna;
using namespace std;

void WindowsApplication::Initialize(std::shared_ptr<ApplicationInitParam> param)
{
    Application::Initialize(param);
	InitMainWindow(param->WindowWidth, param->WindowHeight);
	InitRenderer(param->RenderAPIType, param->WindowWidth, param->WindowHeight);
    SceneManager::Instance->Initialize();
    if (param->fromEditor)
    {
        GUI::Initialize(true, true);
    }
    else
    {
        GUI::Initialize(false, false);
    }

}

void WindowsApplication::Finalize()
{
	GUI::Finalize();
    SceneManager::Instance->Finalize();
    Renderer::Current->Finalize();
}


void WindowsApplication::Tick()
{
	MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HWND WindowsApplication::GetWindowsHandler() noexcept
{
	return m_HWND;
}

LRESULT WindowsApplication::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GUI::HandleEvents(hWnd, message, wParam, lParam))
	{
		return true;
	}


	auto app = reinterpret_cast<WindowsApplication*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	switch (message)
	{
		case WM_CREATE:
		{
			LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
			break;
		}
		case WM_DESTROY:
		{
			app->m_Quit = true;
			break;
		}
		case WM_SIZE:
		{
			if (wParam != SIZE_MINIMIZED)
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
                app->OnWindowSizeChange((int)width, (int)height);
			}
			break;
		}
		case WM_DPICHANGED:
		{
			GUI::HandleMultiViewport(hWnd, lParam);
			break;
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void WindowsApplication::InitMainWindow(int width, int height)
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

	RECT windowRect = { 0, 0, width, height };
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

void WindowsApplication::InitRenderer(RenderAPIType api, int width, int height)
{
	ASUNA_ASSERT(Renderer::Current == nullptr);
    if (api == RenderAPIType::Directx11)
	{
		Renderer::Current = new DirectX11Renderer();
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
	param.m_ResolutionWith = width;
	param.m_ResolutionHeight = height;
	Renderer::Current->Initialize(param);
}

void WindowsApplication::OnWindowSizeChange(int width, int height)
{
    if (Renderer::Current != nullptr)
    {
        Renderer::Current->ResizeResolution(width, height);
    }
}

#endif

