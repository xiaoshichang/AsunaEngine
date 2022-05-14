//
// Created by xiao on 2022/5/7.
//

#pragma once
#include <vector>
#include <memory>

#include "RenderPassBase.h"
#include "../../Graphics/Abstract/RenderItem.h"
#include "../../Graphics/Abstract/RenderTarget.h"
#include "../../GameObject/GameObject.h"

namespace asuna
{
    class RenderPassShadowMap : public RenderPassBase
    {
    public:
        RenderPassShadowMap();
        void Render();
        const std::shared_ptr<RenderTarget>& GetShadowMap() { return m_ShadowMap; }

    private:
        void InitShadowMapRT();
        void InitShadowMapMaterial();
        void CollectRenderItems();
        void BuildRenderQueueVisitGameObject(GameObject* node);

    private:
        std::vector<RenderItem const*> m_Items;
        std::shared_ptr<RenderTarget> m_ShadowMap = nullptr;
        std::shared_ptr<Material> m_ShadowMapMaterial = nullptr;
    };

}
