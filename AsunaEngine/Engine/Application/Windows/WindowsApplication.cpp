#include "WindowsApplication.h"

#ifdef ASUNA_PLATFORM_WINDOWS
#include "../../Foundation/Platform/Assert.h"
#include "../../Foundation/Math/AMath.h"
#include "../../Graphics/Abstract/Renderer.h"
#include "../../Graphics/DirectX11/DirectX11Renderer.h"
#include "../../Graphics/Opengl/OpenglRenderer.h"
#include "../../GUI/GUI.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/RenderPass/RenderPassMgr.h"
#include "../../AssetLoader/AssetLoader.h"


using namespace asuna;
using namespace std;

void WindowsApplication::Initialize(std::shared_ptr<ApplicationInitParam> param)
{
    Application::Initialize(param);
	InitMainWindow(param->WindowWidth, param->WindowHeight);
	InitRenderer(param->RenderAPIType, param->WindowWidth, param->WindowHeight);

    SceneManager::Instance = new SceneManager();
    SceneManager::Instance->Initialize();

    if (param->EditorMode)
    {
        GUI::Initialize(true, true);
    }
    else
    {
        GUI::Initialize(false, false);
    }
    SetupApplicationTitle(param);

}

void WindowsApplication::Finalize()
{
	GUI::Finalize();

    SceneManager::Instance->Finalize();
    delete SceneManager::Instance;
    SceneManager::Instance = nullptr;

    delete RenderPassMgr::Instance;
    RenderPassMgr::Instance = nullptr;

    Renderer::Instance->Finalize();
    delete Renderer::Instance;
    Renderer::Instance = nullptr;
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
	windowClass.lpszClassName = "Engine";
	RegisterClassEx(&windowClass);

	RECT windowRect = { 0, 0, width, height };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window and store a handle to it.
	m_HWND = CreateWindowEx(
		NULL,
		windowClass.lpszClassName,
		"Engine",
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
	ASUNA_ASSERT(Renderer::Instance == nullptr);
    if (api == RenderAPIType::Directx11)
	{
		Renderer::Instance = new DirectX11Renderer();
	}
	else if (api == RenderAPIType::Opengl)
	{
		Renderer::Instance = new OpenglRenderer();
	}
	else
	{
		ASUNA_ASSERT(false);
	}

	CreateRendererContextParam param{};
	param.m_HWND = m_HWND;
	param.m_ResolutionWith = width;
	param.m_ResolutionHeight = height;
	Renderer::Instance->Initialize(param);
    Renderer::Instance->SetViewPort(0, 0, width, height);

    RenderPassMgr::Instance = new RenderPassMgr();
    RenderPassMgr::Instance->Init(param);
}

void WindowsApplication::OnWindowSizeChange(int width, int height)
{
    if (Renderer::Instance != nullptr)
    {
        Renderer::Instance->ResizeResolution(width, height);
        if (!m_Config->EditorMode)
        {
            RenderPassMgr::Instance->ResizeResolution(width, height);
        }
    }
}

void WindowsApplication::SetWindowTitle(char* title)
{
    SetWindowText(m_HWND, title);
}

void WindowsApplication::SetupApplicationTitle(std::shared_ptr<ApplicationInitParam> param)
{
    char title[128];
    char* mode;
    char *render = nullptr;
    if (param->EditorMode)
    {
        mode = "Editor";
    }
    else
    {
        mode = "Player";
    }
    if (param->RenderAPIType == RenderAPIType::Directx11)
    {
        render = "DX11";
    }
    else if (param->RenderAPIType == RenderAPIType::Opengl)
    {
        render = "Opengl";
    }
    sprintf(title, "Engine - [mode:%s] - [render:%s]", mode, render);
    SetWindowTitle(title);
}

#endif

