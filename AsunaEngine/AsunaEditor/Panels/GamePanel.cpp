//
// Created by xiao on 2022/3/28.
//
#include <memory>
#include <imgui.h>
#include "GamePanel.h"
#include "AsunaEngine/Graphics/Renderer.h"
#include "AsunaEngine/Graphics/DirectX11/DirectX11RenderTarget.h"

using namespace asuna;

void GamePanel::Initialize()
{
    auto mesh = Renderer::Current->CreateMesh("Assets\\Models\\keqin.fbx");
    auto vertexShader = Renderer::Current->CreateShader("Assets\\Shaders\\triangle.vs", ShaderType::VertexShader);
    auto pixelShader = Renderer::Current->CreateShader("Assets\\Shaders\\triangle.ps", ShaderType::PixelShader);
    auto constantBuffer = Renderer::Current->CreateConstantBuffer();
    auto renderItem = Renderer::Current->CreateRenderItem(mesh, vertexShader, pixelShader, constantBuffer);
    Renderer::Current->AddRenderItem(renderItem);
}

void GamePanel::Render()
{
    if (m_RT != nullptr)
    {
        Renderer::Current->RenderScene(m_RT);
    }

    ImGui::Begin("Game Window", &m_Showing);
    // Get the size of the child (i.e. the whole draw size of the windows).
    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    ImVec2 vMax = ImGui::GetWindowContentRegionMax();
    auto width = (int)(vMax.x - vMin.x);
    auto height = (int)(vMax.y - vMin.y);

    if (m_RT == nullptr)
    {
        RenderTargetDesc desc{0};
        desc.width = 1600;
        desc.height = 1200;
        m_RT = Renderer::Current->CreateRenderTarget(desc);
    }

    auto rt = std::dynamic_pointer_cast<DirectX11RenderTarget>(m_RT);
    auto tid = (ImTextureID)rt->GetSRV();
    ImVec2 winSize = {(float)width, (float)height};
    ImGui::Image(tid, winSize);
    ImGui::End();
}

