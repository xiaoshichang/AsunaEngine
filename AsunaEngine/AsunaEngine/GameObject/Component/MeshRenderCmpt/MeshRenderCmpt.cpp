//
// Created by xiao on 2022/4/3.
//

#include "MeshRenderCmpt.h"
#include "../../../Graphics/Renderer.h"

void asuna::MeshRenderCmpt::Initialize()
{
    auto cb = Renderer::Current->CreateConstantBuffer();
    m_RenderItem = Renderer::Current->CreateRenderItem(nullptr, nullptr, nullptr, cb);
    Renderer::Current->AddRenderItem(m_RenderItem);
}

void asuna::MeshRenderCmpt::Finalize()
{
    Renderer::Current->RemoveRenderItem(m_RenderItem);
}

void asuna::MeshRenderCmpt::Tick()
{
}

void asuna::MeshRenderCmpt::SetMesh(const std::string &path)
{
    m_MeshPath = path;
    auto mesh = Renderer::Current->CreateMesh(path);
    m_RenderItem->SetMesh(mesh);
}

void asuna::MeshRenderCmpt::SetMaterial(const std::string &vsPath, const std::string &psPath)
{
    m_VSPath = vsPath;
    m_PSPath = psPath;
    auto vs = Renderer::Current->CreateShader(vsPath, ShaderType::VertexShader);
    auto ps = Renderer::Current->CreateShader(psPath, ShaderType::PixelShader);
    m_RenderItem->SetVertexShader(vs);
    m_RenderItem->SetPixelShader(ps);
}

