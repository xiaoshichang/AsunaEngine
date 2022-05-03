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
    EditorEventCallback callback1 = std::bind(&InspectorPanel::OnSelectNewGameObject, this, std::placeholders::_1, std::placeholders::_2);
    EditorEventManager::Register(EditorEventType::OnHierarchPanelSelectNewObject, this, callback1);
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
    EditorEventCallback callback2 = std::bind(&InspectorPanel::OnSelectNewGameObject, this, std::placeholders::_1, std::placeholders::_2);
    EditorEventManager::Unregister(EditorEventType::OnHierarchPanelSelectNewObject, this, callback2);
}

void InspectorPanel::OnSelectNewGameObject(EditorEventType type, const void* param)
{
    m_InspectingGameObject = (GameObject*)param;
    m_TransformCmpt = m_InspectingGameObject->GetTransform();
    m_CameraCmpt = m_InspectingGameObject->GetComponent<CameraCmpt>();
    m_MeshRenderCmpt = m_InspectingGameObject->GetComponent<MeshRenderCmpt>();
    m_LightRenderCmpt = m_InspectingGameObject->GetComponent<LightCmpt>();
}


