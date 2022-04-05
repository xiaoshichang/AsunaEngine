//
// Created by xiao on 2022/3/28.
//
#include <memory>
#include <imgui.h>
#include "GamePanel.h"
#include "AsunaEngine/Graphics/Renderer.h"
#include "AsunaEngine/Graphics/DirectX11/DirectX11RenderTarget.h"
#include "AsunaEngine/Scene/SceneManager.h"
#include "../Events/EditorEventManager.h"

using namespace asuna;

static bool first = true;

void GamePanel::Initialize()
{
}

void GamePanel::Render()
{
    if (first)
    {
        first = false;
        return;
    }
    ImGui::Begin("Game Window", &m_Showing);
    RenderResolutionOptions();
    RenderSceneToRT();
    RenderRTTOWindow();
    ImGui::End();
}


void GamePanel::Finalize()
{

}

void GamePanel::RenderResolutionOptions()
{
    const char* Resolutions[] = {"1600x1200", "1200x960", "1200x800"};
    ImGui::Combo("Resolution", &m_SelectedResolution, Resolutions, IM_ARRAYSIZE(Resolutions), IM_ARRAYSIZE(Resolutions));
    int targetWidth, targetHeight;
    if (m_SelectedResolution == 0)
    {
        m_TargetResolutionWidth = 1600;
        m_TargetResolutionHeight = 1200;
    }
    else if(m_SelectedResolution == 1)
    {
        m_TargetResolutionWidth = 1200;
        m_TargetResolutionHeight = 960;
    }
    else
    {
        m_TargetResolutionWidth = 1200;
        m_TargetResolutionHeight = 800;
    }
    if (m_RT == nullptr || m_SelectedResolution != m_LastResolution)
    {
        ResizeRT();
        SceneManager::Instance->GetMainCamera()->SetAspectRatio((float)m_TargetResolutionWidth / (float)m_TargetResolutionHeight);
        m_LastResolution = m_SelectedResolution;
    }
}

void GamePanel::RenderSceneToRT()
{
    Renderer::Current->SetViewPort(0, 0, m_TargetResolutionWidth, m_TargetResolutionHeight);
    SceneManager::Instance->Render(m_RT);
}

void GamePanel::RenderRTTOWindow()
{
    float optionHeight = 20;
    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    ImVec2 vMax = ImGui::GetWindowContentRegionMax();
    auto winWidth = (vMax.x - vMin.x);
    auto winHeight = (vMax.y - vMin.y) - optionHeight;
    float warpWidth, warpHeight;
    if ((float)m_TargetResolutionWidth / (float)m_TargetResolutionWidth > winWidth / winHeight)
    {
        warpWidth = (float)winWidth * 0.95;
        warpHeight = (float)winWidth * (float)m_TargetResolutionHeight / (float)m_TargetResolutionWidth * 0.95;
    }
    else
    {
        warpWidth = (float)winHeight * (float)m_TargetResolutionWidth / (float)m_TargetResolutionHeight * 0.95;
        warpHeight = (float)winHeight * 0.95;
    }
    ImGui::SetCursorPosX((float)winWidth / 2 - warpWidth / 2 + 10);
    ImGui::SetCursorPosY((float)(vMax.y - vMin.y) / 2 - warpHeight / 2 + optionHeight * 2);
    ImVec2 winSize = {warpWidth, warpHeight};
    auto rt = std::dynamic_pointer_cast<DirectX11RenderTarget>(m_RT);
    auto tid = (ImTextureID)rt->GetSRV();
    ImGui::Image(tid, winSize);
}

void GamePanel::ResizeRT()
{
    RenderTargetDesc desc{0};
    desc.width = m_TargetResolutionWidth;
    desc.height = m_TargetResolutionHeight;
    m_RT = Renderer::Current->CreateRenderTarget(desc);
}

