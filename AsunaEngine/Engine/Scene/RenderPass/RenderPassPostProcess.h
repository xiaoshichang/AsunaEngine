//
// Created by xiao on 2022/5/7.
//
#pragma once
#include <memory>
#include "RenderPassBase.h"
#include "../../Graphics/Abstract/Mesh.h"
#include "../../Graphics/Abstract/RenderTarget.h"
#include "../../Graphics/Abstract/RenderItem.h"

namespace asuna
{
    class RenderPassPostProcess : public RenderPassBase
    {
    public:
        RenderPassPostProcess();
        void Render(const std::shared_ptr<RenderTarget>& inputRT, const std::shared_ptr<RenderTarget>& outputRT);

    private:
        void CreateRenderItem();
        void CreateMaterial();

    private:
        std::shared_ptr<Material> m_Material;
        std::shared_ptr<RenderItem> m_RenderItem;
        std::shared_ptr<Mesh> m_Mesh;
        std::shared_ptr<ConstantBuffer> m_ConstantBuffer;
    };

}
