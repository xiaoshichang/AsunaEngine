//
// Created by xiao on 2022/3/28.
//

#include "InspectorPanel.h"
#include <imgui.h>
#include "Engine/GameObject/GameObject.h"
#include "Engine/Foundation/Logger/Logger.h"


using namespace asuna;

void InspectorPanel::Initialize()
{
}

void InspectorPanel::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(100, 250));
    ImGui::Begin("Inspector Window", &m_Showing);
    if (m_InspectingGameObject != nullptr)
    {
        ImGui::Text("%s", m_InspectingGameObject->GetName().c_str());
        TransformCmptEditor::Render(m_TransformCmpt);
        CameraCmptEditor::Render(m_CameraCmpt);
        MeshRenderEditor::Render(m_MeshRenderCmpt);
        LightCmptEditor::Render(m_LightRenderCmpt);
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void InspectorPanel::Finalize()
{
}

void InspectorPanel::OnSelectNewGameObject(GameObject* target)
{
    m_InspectingGameObject = target;
    m_TransformCmpt = m_InspectingGameObject->GetTransform();
    m_CameraCmpt = m_InspectingGameObject->GetComponent<CameraCmpt>();
    m_MeshRenderCmpt = m_InspectingGameObject->GetComponent<MeshRenderCmpt>();
    m_LightRenderCmpt = m_InspectingGameObject->GetComponent<LightCmpt>();
}


