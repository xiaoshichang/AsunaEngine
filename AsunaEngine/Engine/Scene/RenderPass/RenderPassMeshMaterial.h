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

namespace asuna
{
    class RenderPassMeshMaterial : public RenderPassBase
    {
    public:
        RenderPassMeshMaterial();
        void Render(const std::shared_ptr<RenderTarget>& rt);

    private:
        void CollectRenderItems();
        void BuildRenderQueueVisitGameObject(GameObject* node);
        void CreateCoordAxisRenderItem();

    private:
        std::vector<RenderItem const*> m_Items;
        bool m_ShowCoordAxis = true;
        std::vector<std::shared_ptr<RenderItem>> m_AxisRenderItems;
    };

}
