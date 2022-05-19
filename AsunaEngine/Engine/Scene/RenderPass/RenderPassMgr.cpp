//
// Created by xiao on 2022/5/7.
//

#include "RenderPassMgr.h"
#include "../../Graphics/Abstract/Renderer.h"

using namespace asuna;

RenderPassMgr* RenderPassMgr::Instance = nullptr;

RenderPassMgr::RenderPassMgr() = default;

RenderPassMgr::~RenderPassMgr()
{
    delete m_RenderPassMeshMaterial;
    m_RenderPassMeshMaterial = nullptr;

    delete m_RenderPassShadowMap;
    m_RenderPassShadowMap = nullptr;

    delete m_RenderPassPostProcess;
    m_RenderPassPostProcess = nullptr;
}

void RenderPassMgr::Init(CreateRendererContextParam param)
{
    m_RenderPassShadowMap = new RenderPassShadowMap();
    m_RenderPassMeshMaterial = new RenderPassMeshMaterial(param);
    m_RenderPassPostProcess = new RenderPassPostProcess();
}

void RenderPassMgr::RenderMeshMaterials(const std::shared_ptr<RasterizationState>& rs)
{
    ASUNA_ASSERT(m_RenderPassMeshMaterial != nullptr);
    auto shadowMap = m_RenderPassShadowMap->GetShadowMap();
    m_RenderPassMeshMaterial->SetShadowMap(shadowMap);
    m_RenderPassMeshMaterial->Render(rs);
}

void RenderPassMgr::RenderShadowMap()
{
    m_RenderPassShadowMap->Render();
}

void RenderPassMgr::RenderPostProcess(const std::shared_ptr<RenderTarget>& output)
{
    auto input = m_RenderPassMeshMaterial->GetMainRT();
    m_RenderPassPostProcess->Render(input, output);
}

void RenderPassMgr::ResizeResolution(int width, int height)
{
    m_RenderPassMeshMaterial->ResizeResolution(width, height);
    m_RenderPassPostProcess->ResizeResolution(width, height);
}

void RenderPassMgr::SetPostProcessEffect(PostProcessEffect effect)
{
    m_RenderPassPostProcess->SetPostProcessEffect(effect);
}

PostProcessEffect RenderPassMgr::GetPostProcessEffect()
{
    return m_RenderPassPostProcess->GetPostProcessEffect();
}




