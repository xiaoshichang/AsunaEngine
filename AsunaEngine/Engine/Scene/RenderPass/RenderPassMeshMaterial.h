//
// Created by xiao on 2022/5/7.
//

#pragma once
#include <memory>
#include <vector>
#include "RenderPassBase.h"
#include "../../GameObject/GameObject.h"
#include "../../Graphics/Abstract/RenderItem.h"
#include "../../Graphics/Abstract/RenderTarget.h"
#include "../../Graphics/Abstract/Renderer.h"
#include "../../Graphics/Abstract/RasterizationState.h"


namespace asuna
{
    class RenderPassMeshMaterial : public RenderPassBase
    {
    public:
        explicit RenderPassMeshMaterial(CreateRendererContextParam param);
        void Render(const std::shared_ptr<RasterizationState>& rs);
        void ResizeResolution(int width, int height) override;
        void SetShadowMap(const std::shared_ptr<RenderTarget>& shadowMap);
        const std::shared_ptr<RenderTarget>& GetMainRT();
    private:
        void CollectRenderItems();
        void BuildRenderQueueVisitGameObject(GameObject* node);
        void CreateCoordAxisRenderItem();

    private:
        std::vector<RenderItem const*> m_Items;
        bool m_ShowCoordAxis = true;
        std::vector<std::shared_ptr<RenderItem>> m_AxisRenderItems;
        std::shared_ptr<RenderTarget> m_MainRT = nullptr;
        std::shared_ptr<RenderTarget> m_ShadowMap = nullptr;
    };

}
