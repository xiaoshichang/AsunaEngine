//
// Created by xiao on 2022/3/28.
//

#pragma once
#include <memory>
#include "AsunaEngine/Graphics/RenderTarget.h"
#include "BasePanel.h"

namespace asuna
{
    class GamePanel : public  BasePanel
    {
    public:
        GamePanel() :
            m_RT(nullptr)
        {
        }

        void Initialize() override;
        void Render() override;

    private:
        void CreateRenderTarget();

    private:
        std::shared_ptr<RenderTarget> m_RT;
    };
}




