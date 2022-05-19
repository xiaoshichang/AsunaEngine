//
// Created by xiao on 2022/3/28.
//
#include <memory>
#include <imgui.h>
#include "GamePanel.h"
#include "Engine/Graphics/Abstract/Renderer.h"
#include "Engine/Scene/RenderPass/RenderPassMgr.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Foundation/Platform/Assert.h"
#include "Engine/Foundation/Platform/Platform.h"

#if defined(ASUNA_PLATFORM_WINDOWS)
#include "Engine/Graphics/DirectX11/DirectX11RenderTarget.h"
#endif

#include "Engine/Graphics/Opengl/OpenglRenderTarget.h"

#pragma warning(disable: 4312)

using namespace asuna;

static bool first = true;

const char* Resolutions[] = {"1600x1200", "1200x960", "1200x800"};
const char* PostProcessEffects[] = {"None", "Gray"};
const char* RasterizationEffect[] = {"Default", "WireFrame"};

void GamePanel::Initialize()
{
    RasterizationStateDesc desc{};
    desc.CullMode = RasterizationCullMode::Back;
    desc.FillMode = RasterizationFillMode::WireFrame;
    desc.FrontCounterClockWise = false;
    m_WireFrameRS = Renderer::Instance->CreateRasterizationState(desc);
}

void GamePanel::Render()
{
    if (first)
    {
        first = false;
        return;
    }
    ImGui::Begin("Game Window", &m_Showing);
    ImGui::PushItemWidth(100);
    RenderResolutionOptions();
    ImGui::SameLine();
    RenderPostProcessOptions();
    ImGui::SameLine();
    RenderRasterizationStateOptions();
    ImGui::PopItemWidth();
    RenderSceneToRT();
    RenderRTTOWindow();
    ImGui::End();
}


void GamePanel::Finalize()
{

}

void GamePanel::RenderResolutionOptions()
{
    ImGui::Combo("Resolution", &m_SelectedResolution, Resolutions, IM_ARRAYSIZE(Resolutions), IM_ARRAYSIZE(Resolutions));
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
    else if(m_SelectedResolution == 2)
    {
        m_TargetResolutionWidth = 1200;
        m_TargetResolutionHeight = 800;
    }
    else
    {
        Logger::Error("unknown resolution type!");
        ASUNA_ASSERT(false);
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
    Renderer::Instance->SetViewPort(0, 0, m_TargetResolutionWidth, m_TargetResolutionHeight);
    if (m_LastRasterization == 0)
    {
        SceneManager::Instance->Render(m_RT, nullptr);
    }
    else if (m_LastRasterization == 1)
    {
        SceneManager::Instance->Render(m_RT, m_WireFrameRS);
    }
    else
    {
        ASUNA_ASSERT(false);
    }

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
        warpWidth = (float)winWidth * 0.95f;
        warpHeight = (float)winWidth * (float)m_TargetResolutionHeight / (float)m_TargetResolutionWidth * 0.95f;
    }
    else
    {
        warpWidth = (float)winHeight * (float)m_TargetResolutionWidth / (float)m_TargetResolutionHeight * 0.95f;
        warpHeight = (float)winHeight * 0.95f;
    }
    ImGui::SetCursorPosX((float)winWidth / 2 - warpWidth / 2 + 10);
    ImGui::SetCursorPosY((float)(vMax.y - vMin.y) / 2 - warpHeight / 2 + optionHeight * 2);
    ImVec2 winSize = {warpWidth, warpHeight};

    if (Renderer::Instance->m_APIType == RenderAPIType::Opengl)
    {
        auto rt = std::dynamic_pointer_cast<OpenglRenderTarget>(m_RT);
        auto tid = reinterpret_cast<ImTextureID>(rt->GetTexture());
        // https://github.com/cinder/Cinder/issues/2185
        ImVec2 uv0 = {0, 1};
        ImVec2 uv1 = {1, 0};
        ImGui::Image(tid, winSize, uv0, uv1);
    }
#if defined(ASUNA_PLATFORM_WINDOWS)
    else if (Renderer::Instance->m_APIType == RenderAPIType::Directx11)
    {
        auto rt = std::dynamic_pointer_cast<DirectX11RenderTarget>(m_RT);
        auto tid = (ImTextureID)rt->GetSRV();
        ImGui::Image(tid, winSize);
    }
#endif
    else
    {
        ASUNA_ASSERT(false);
    }
}

void GamePanel::ResizeRT()
{
    if (m_RT == nullptr)
    {
        RenderTargetDesc desc{};
        desc.usage = RenderTargetUsage::Default;
        desc.width = m_TargetResolutionWidth;
        desc.height = m_TargetResolutionHeight;
        m_RT = Renderer::Instance->CreateRenderTarget(desc);
    }
    else
    {
        m_RT->Resize(m_TargetResolutionWidth, m_TargetResolutionHeight);
    }

    RenderPassMgr::Instance->ResizeResolution(m_TargetResolutionWidth, m_TargetResolutionHeight);
}

void GamePanel::RenderPostProcessOptions()
{
    int cur = (int)RenderPassMgr::Instance->GetPostProcessEffect();
    ImGui::Combo("PostProcess", &cur, PostProcessEffects, IM_ARRAYSIZE(PostProcessEffects), IM_ARRAYSIZE(PostProcessEffects));
    RenderPassMgr::Instance->SetPostProcessEffect((PostProcessEffect)cur);
}

void GamePanel::RenderRasterizationStateOptions()
{
    ImGui::Combo("Rasterization", &m_LastRasterization, RasterizationEffect, IM_ARRAYSIZE(RasterizationEffect), IM_ARRAYSIZE(RasterizationEffect));
}

