//
// Created by xiao on 2022/3/28.
//


#pragma once
#include "BasePanel.h"
#include "Engine/GameObject/GameObject.h"
#include "../Component/TransformCmptEditor.h"
#include "../Component/CameraCmptEditor.h"
#include "../Component/MeshRenderEditor.h"
#include "../Component/LightCmptEditor.h"

namespace asuna
{
    class InspectorPanel : public BasePanel
    {
    public:
        void Initialize() override;
        void Render() override;
        void Finalize() override;
        void OnSelectNewGameObject(GameObject* target);

    private:
        GameObject* m_InspectingGameObject = nullptr;
        TransformCmpt* m_TransformCmpt = nullptr;
        CameraCmpt* m_CameraCmpt = nullptr;
        MeshRenderCmpt* m_MeshRenderCmpt = nullptr;
        LightCmpt* m_LightRenderCmpt = nullptr;
    };

}



