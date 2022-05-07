//
// Created by xiao on 2022/5/7.
//

#include "RenderPassMgr.h"
#include "../../Graphics/Abstract/Renderer.h"

using namespace asuna;

RenderPassMgr* RenderPassMgr::Instance = nullptr;

RenderPassMgr::RenderPassMgr()
{
    m_RenderPassMeshMaterial = new RenderPassMeshMaterial();
    m_RenderPassShadowMap = new RenderPassShadowMap();
    m_RenderPassPostProcess = new RenderPassPostProcess();
}

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
    RenderTargetDesc desc{};
    desc.usage = RenderTargetUsage::Default;
    desc.width = param.m_ResolutionWith;
    desc.height = param.m_ResolutionHeight;
    m_MainRT = Renderer::Instance->CreateRenderTarget(desc);
}

void RenderPassMgr::RenderMeshMaterials( const std::shared_ptr<RenderTarget>& rt)
{
    ASUNA_ASSERT(m_RenderPassMeshMaterial != nullptr);
    m_RenderPassMeshMaterial->Render(rt);
}

void RenderPassMgr::RenderShadowMap()
{
    // todo
}

void RenderPassMgr::RenderPostProcess(const std::shared_ptr<RenderTarget>& rt)
{
    // todo
}

void RenderPassMgr::ResizeResolution(int width, int height)
{
    m_MainRT->Resize(width, height);
}


