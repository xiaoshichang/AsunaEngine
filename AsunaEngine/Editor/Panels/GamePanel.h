//
// Created by xiao on 2022/3/28.
//

#pragma once
#include <memory>
#include <Editor/Events/EditorEventType.h>
#include "Engine/Graphics/Abstract/RenderTarget.h"
#include "BasePanel.h"
#include "Engine/GameObject/GameObject.h"

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
        void Finalize() override;

    private:
        void RenderResolutionOptions();
        void ResizeRT();
        void RenderSceneToRT();
        void RenderRTTOWindow();

    private:
        int m_SelectedResolution = 0;
        int m_LastResolution = 0;
        int m_TargetResolutionWidth = 0;
        int m_TargetResolutionHeight = 0;
        std::shared_ptr<RenderTarget> m_RT;
    };
}



