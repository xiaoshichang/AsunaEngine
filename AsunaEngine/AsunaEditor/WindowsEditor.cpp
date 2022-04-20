#include "WindowsEditor.h"
#include "AsunaEngine/Graphics/Abstract/Renderer.h"
#include "AsunaEngine/GUI/GUI.h"
#include "AsunaEngine/Scene/SceneManager.h"
#include <imgui.h>


using namespace asuna;

static void ShowDockingDisabledMessage()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
    ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
    ImGui::SameLine(0.0f, 0.0f);
    if (ImGui::SmallButton("click here"))
    {
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }
}


void WindowsEditor::Initialize(std::shared_ptr<ApplicationInitParam> param)
{
    WindowsApplication::Initialize(param);
    m_HierarchyPanel.Initialize();
    m_LogPanel.Initialize();
    m_GamePanel.Initialize();
    m_InspectorPanel.Initialize();
    SceneManager::Instance->LoadScene("test");
    Logger::Info("Editor Init OK!");
}


void asuna::WindowsEditor::Render()
{
	GUI::Begin();
    //ImGui::ShowDemoWindow();
    RenderEditorBegin();
    m_HierarchyPanel.Render();
    m_LogPanel.Render();
    m_GamePanel.Render();
    m_InspectorPanel.Render();
    RenderEditorEnd();
	GUI::End();
	Renderer::Current->Present();
}

void WindowsEditor::RenderEditorEnd()
{
    ImGui::End();
}

void WindowsEditor::RenderEditorBegin()
{
    static bool open = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }
    if (!opt_padding)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    }

    ImGui::Begin("DockSpace Demo", &open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        ShowDockingDisabledMessage();
    }

}

void WindowsEditor::Finalize()
{
    m_HierarchyPanel.Finalize();
    m_LogPanel.Finalize();
    m_GamePanel.Finalize();
    m_InspectorPanel.Finalize();
    WindowsApplication::Finalize();
}

void WindowsEditor::Tick()
{
    WindowsApplication::Tick();
    SceneManager::Instance->Tick();
}

void WindowsEditor::OnWindowSizeChange(int width, int height)
{
    WindowsApplication::OnWindowSizeChange(width, height);
}

