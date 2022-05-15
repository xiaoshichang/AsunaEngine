//
// Created by xiao on 2022/5/7.
//

#include "RenderPassShadowMap.h"
#include "../../Graphics/Abstract/Renderer.h"
#include "../SceneManager.h"
#include "../../GameObject/Component/MeshRenderCmpt/MeshRenderCmpt.h"

using namespace asuna;

const int SHADOW_MAP_WIDTH = 2048;
const int SHADOW_MAP_HEIGHT = 2048;


void RenderPassShadowMap::InitShadowMapRT()
{
    RenderTargetDesc desc{};
    desc.usage = RenderTargetUsage::ShadowMap;
    desc.width = SHADOW_MAP_WIDTH;
    desc.height = SHADOW_MAP_HEIGHT;
    m_ShadowMap = Renderer::Instance->CreateRenderTarget(desc);
}

void RenderPassShadowMap::InitShadowMapMaterial()
{
    m_ShadowMapMaterial = Renderer::Instance->CreateMaterial("Global_ShadowMapping", MaterialType::ShadowMapping);
}

RenderPassShadowMap::RenderPassShadowMap()
{
    InitShadowMapRT();
    InitShadowMapMaterial();
}

void RenderPassShadowMap::Render()
{
    SceneManager::Instance->GetConstantBufferPerScene()->Bind();
    Renderer::Instance->SetRenderTarget(m_ShadowMap);
    Renderer::Instance->ClearRenderTarget(m_ShadowMap, 0.1f, 0.2f, 0.3f, 1.0f);
    Renderer::Instance->SetViewPort(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
    CollectRenderItems();
    for(auto item : m_Items)
    {
        item->Render(m_ShadowMapMaterial);
    }
    // unbind shadow map so next render pass can use it as input texture.
    Renderer::Instance->SetRenderTarget(nullptr);
}

void RenderPassShadowMap::CollectRenderItems()
{
    m_Items.clear();
    auto root = SceneManager::Instance->GetRoot();
    BuildRenderQueueVisitGameObject(root.get());
}

void RenderPassShadowMap::BuildRenderQueueVisitGameObject(GameObject *node)
{
    auto meshRender = node->GetComponent<MeshRenderCmpt>();
    if (meshRender != nullptr && meshRender->GetCastShadowFlag())
    {
        m_Items.push_back(meshRender->GetRenderItem());
    }
    for(auto& child : node->GetTransform()->GetChildren())
    {
        BuildRenderQueueVisitGameObject(child->GetOwner());
    }
}







