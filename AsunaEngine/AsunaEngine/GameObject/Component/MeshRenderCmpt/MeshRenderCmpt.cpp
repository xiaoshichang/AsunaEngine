//
// Created by xiao on 2022/4/3.
//

#include "MeshRenderCmpt.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Graphics/Abstract/Renderer.h"

using namespace asuna;
using namespace std;

void MeshRenderCmpt::Initialize()
{
    m_ConstantBufferPerObject = Renderer::Current->CreateConstantBuffer(ConstantBufferDataType::PerObject, sizeof(ConstantBufferDataPerObject));
    m_RenderItem = Renderer::Current->CreateRenderItem(nullptr, m_ConstantBufferPerObject);
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

void MeshRenderCmpt::SetMesh(const std::shared_ptr<Mesh>& mesh)
{
    m_RenderItem->SetMesh(mesh);
    m_RenderItem->AllocateMaterials(mesh->GetMaterialCount());
}

int MeshRenderCmpt::GetMaterialCount() const
{
    return m_RenderItem->GetMaterialCount();
}

std::shared_ptr<Material> MeshRenderCmpt::GetMaterial(int index)
{
    return m_RenderItem->GetMaterial(index);
}

void MeshRenderCmpt::SetMaterial(int index, const std::shared_ptr<Material>& material)
{
    m_RenderItem->SetMaterial(index, material);
}

void MeshRenderCmpt::UpdateConstantBufferPerObject()
{
    auto perObject = (ConstantBufferDataPerObject*)m_ConstantBufferPerObject->GetData();
    perObject->m_WorldMatrix = GetOwner()->GetTransform()->GetRTSMatrix();
}






