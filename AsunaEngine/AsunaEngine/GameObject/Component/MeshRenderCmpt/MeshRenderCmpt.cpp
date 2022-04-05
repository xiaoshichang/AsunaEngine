//
// Created by xiao on 2022/4/3.
//

#include "MeshRenderCmpt.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Graphics/Renderer.h"

using namespace asuna;

void MeshRenderCmpt::Initialize()
{
    m_ConstantBufferPerObject = Renderer::Current->CreateConstantBuffer(ConstantBufferDataType::PerObject);
    auto perScene = SceneManager::Instance->GetConstantBufferPerScene();
    m_RenderItem = Renderer::Current->CreateRenderItem(nullptr, nullptr, nullptr, m_ConstantBufferPerObject, perScene);
    SceneManager::Instance->AddRenderItem(m_RenderItem);
}

void MeshRenderCmpt::Finalize()
{
    SceneManager::Instance->RemoveRenderItem(m_RenderItem);
}

void MeshRenderCmpt::Tick()
{
    UpdateConstantBufferPerObject();
}

void MeshRenderCmpt::SetMesh(const std::string &path)
{
    m_MeshPath = path;
    auto mesh = Renderer::Current->CreateMesh(path);
    m_RenderItem->SetMesh(mesh);
}

void MeshRenderCmpt::SetMaterial(const std::string &vsPath, const std::string &psPath)
{
    m_VSPath = vsPath;
    m_PSPath = psPath;
    auto vs = Renderer::Current->CreateShader(vsPath, ShaderType::VertexShader);
    auto ps = Renderer::Current->CreateShader(psPath, ShaderType::PixelShader);
    m_RenderItem->SetVertexShader(vs);
    m_RenderItem->SetPixelShader(ps);
}

void MeshRenderCmpt::UpdateConstantBufferPerObject()
{
    auto perObject = (ConstantBufferDataPerObject*)m_ConstantBufferPerObject->GetData();
    perObject->m_WorldMatrix = GetOwner()->GetTransform()->GetRTSMatrix();
}

