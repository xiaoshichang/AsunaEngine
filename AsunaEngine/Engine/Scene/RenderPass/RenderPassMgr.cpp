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
    m_RenderPassMeshMaterial = new RenderPassMeshMaterial(param);
    m_RenderPassShadowMap = new RenderPassShadowMap();
    m_RenderPassPostProcess = new RenderPassPostProcess();
}

void RenderPassMgr::RenderMeshMaterials()
{
    ASUNA_ASSERT(m_RenderPassMeshMaterial != nullptr);
    m_RenderPassMeshMaterial->Render();
}

void RenderPassMgr::RenderShadowMap()
{
    // todo
}

void RenderPassMgr::RenderPostProcess(const std::shared_ptr<RenderTarget>& rt)
{
    auto input = m_RenderPassMeshMaterial->GetMainRT();
    m_RenderPassPostProcess->Render(input, rt);
}

void RenderPassMgr::ResizeResolution(int width, int height)
{
    m_RenderPassMeshMaterial->ResizeResolution(width, height);
    m_RenderPassPostProcess->ResizeResolution(width, height);
}


