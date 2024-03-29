//
// Created by xiao on 2022/5/7.
//

#pragma once
#include <memory>
#include "RenderPassMeshMaterial.h"
#include "RenderPassShadowMap.h"
#include "RenderPassPostProcess.h"
#include "../../Graphics/Abstract/RenderTarget.h"
#include "../../Graphics/Abstract/Renderer.h"

namespace asuna
{
    class RenderPassMgr
    {
    public:
        RenderPassMgr();
        ~RenderPassMgr();
        static RenderPassMgr* Instance;
        void Init(CreateRendererContextParam param);

    public:
        void RenderShadowMap();
        void RenderMeshMaterials(const std::shared_ptr<RasterizationState>& rs);
        void RenderPostProcess(const std::shared_ptr<RenderTarget>& rt);
        void ResizeResolution(int width, int height);

    public:
        void SetPostProcessEffect(PostProcessEffect effect);
        PostProcessEffect GetPostProcessEffect();


    private:
        RenderPassMeshMaterial* m_RenderPassMeshMaterial = nullptr;
        RenderPassShadowMap* m_RenderPassShadowMap = nullptr;
        RenderPassPostProcess* m_RenderPassPostProcess = nullptr;

    };
}




