#include "GUI.h"
#include <memory>
#include <imgui.h>
#include "../Graphics/Abstract/Renderer.h"
#include <imgui_impl_opengl3.h>
#include "../Graphics/Opengl/OpenglRenderer.h"

#ifdef ASUNA_PLATFORM_WINDOWS
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include "../Graphics/DirectX11/DirectX11RenderContext.h"
#endif


using namespace std;
using namespace asuna;

#ifdef ASUNA_PLATFORM_WINDOWS
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif

void GUI::Initialize(bool docking, bool multivp)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    if (docking)
    {
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }
    if (multivp && Renderer::Instance->m_APIType == RenderAPIType::Directx11)
    {
        // ViewportsEnable is not supported when using opengl without glfw in imGui right now.
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    }
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
#ifdef ASUNA_PLATFORM_WINDOWS
    ImGui_ImplWin32_Init(Renderer::Instance->m_HWND);
#endif

    if (Renderer::Instance->m_APIType == RenderAPIType::Directx11)
    {
        auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Instance->GetContext());
        ImGui_ImplDX11_Init(context->m_Device, context->m_DeviceContext);
    }
    else if (Renderer::Instance->m_APIType == RenderAPIType::Opengl)
    {
        ImGui_ImplOpenGL3_Init("#version 420");
    }
    else
    {
        ASUNA_ASSERT(false);
    }
    
}

void GUI::Begin()
{
    // Start the Dear ImGui frame
    if (Renderer::Instance->m_APIType == RenderAPIType::Directx11)
    {
        ImGui_ImplDX11_NewFrame();
    }
    else if(Renderer::Instance->m_APIType == RenderAPIType::Opengl)
    {
        ImGui_ImplOpenGL3_NewFrame();
    }
    else
    {
        ASUNA_ASSERT(false);
    }


#ifdef ASUNA_PLATFORM_WINDOWS
    ImGui_ImplWin32_NewFrame();
#endif

    ImGui::NewFrame();
}

void GUI::End()
{
    ImGui::Render();
    Renderer::Instance->SetViewPort(0, 0, -1, -1);
    Renderer::Instance->SetRenderTarget(nullptr);
    Renderer::Instance->ClearRenderTarget(nullptr, 0.1f, 0.2f, 0.3f, 1.0f);
    Renderer::Instance->SetRasterizationState(nullptr);

    if (Renderer::Instance->GetRenderAPIType() == RenderAPIType::Directx11)
    {
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
    else if (Renderer::Instance->GetRenderAPIType() == RenderAPIType::Opengl)
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    else
    {
        ASUNA_ASSERT(false);
    }

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        auto io = ImGui::GetPlatformIO();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        if (Renderer::Instance->m_APIType == RenderAPIType::Opengl)
        {
            auto renderer = (OpenglRenderer*)Renderer::Instance;
            renderer->MakeCurrentContext();
        }
    }
}

void GUI::Finalize()
{
	ImGui_ImplWin32_Shutdown();

    // Cleanup
    if (Renderer::Instance->GetRenderAPIType() == RenderAPIType::Directx11)
    {
        ImGui_ImplDX11_Shutdown();
    }
    else if (Renderer::Instance->GetRenderAPIType() == RenderAPIType::Opengl)
    {
        ImGui_ImplOpenGL3_Shutdown();
    }
    else
    {
        ASUNA_ASSERT(false);
    }

    ImGui::DestroyContext();
}

HRESULT GUI::HandleEvents(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef ASUNA_PLATFORM_WINDOWS
    return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
#endif
}

void GUI::HandleMultiViewport(HWND hWnd, LPARAM lParam)
{
#ifdef ASUNA_PLATFORM_WINDOWS
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
    {
        //const int dpi = HIWORD(wParam);
        //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
        const RECT* suggested_rect = (RECT*)lParam;
        ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
    }
#endif
}


void GUI::TestGUI()
{
    static float f = 0.0f;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    static bool show;
    ImGui::ShowDemoWindow(&show);
}
