#include <memory>
#include "GUI.h"
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "../Graphics/Renderer.h"
#include "../Graphics/DirectX11/DirectX11RenderContext.h"

using namespace std;
using namespace asuna;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void asuna::GUI::Initialize()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
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
    ImGui_ImplWin32_Init(Renderer::Current->m_HWND);

    if (Renderer::Current->m_APIType == RenderAPIType::Directx11)
    {
        auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Current->GetContext());
        ImGui_ImplDX11_Init(context->m_Device, context->m_DeviceContext);
    }
    else if (Renderer::Current->m_APIType == RenderAPIType::Opengl)
    {
        ASUNA_ASSERT(false);
    }
    else
    {
        ASUNA_ASSERT(false);
    }
    
}

void asuna::GUI::Begin()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void asuna::GUI::End()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void asuna::GUI::Finalize()
{
    // Cleanup
    ImGui::DestroyContext();
}

HRESULT asuna::GUI::HandleEvents(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

void asuna::GUI::HandleMultiViewport(HWND hWnd, LPARAM lParam)
{
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
    {
        //const int dpi = HIWORD(wParam);
        //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
        const RECT* suggested_rect = (RECT*)lParam;
        ::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
    }
}


void asuna::GUI::TestGUI()
{
    static float f = 0.0f;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    static bool show;
    ImGui::ShowDemoWindow(&show);
}
