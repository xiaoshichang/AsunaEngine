#include <memory>
#include "IMGui.h"
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "../Graphics/Renderer.h"
#include "../Graphics/DirectX11/DirectX11RenderContext.h"

using namespace std;
using namespace asuna;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void asuna::IMGui::Initialize()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

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

void asuna::IMGui::Render()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    // gui logic enterance
    TestGUI();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void asuna::IMGui::Finalize()
{
    // Cleanup
    ImGui::DestroyContext();
}

HRESULT asuna::IMGui::HandleEvents(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}


void asuna::IMGui::TestGUI()
{
    static float f = 0.0f;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    static bool show;
    ImGui::ShowDemoWindow(&show);
}
