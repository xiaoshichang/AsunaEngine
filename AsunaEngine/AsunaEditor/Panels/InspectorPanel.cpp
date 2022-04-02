//
// Created by xiao on 2022/3/28.
//

#include "InspectorPanel.h"
#include <imgui.h>
#include "../Component/Transform.h"
#include "AsunaEngine/GameObject/GameObject.h"
#include "AsunaEngine/Foundation/Logger/Logger.h"


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
        ImGui::Text(m_InspectingGameObject->GetName().c_str());
        TransformComponentEditor::Render(m_InspectingGameObject->GetTransform());
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
}


