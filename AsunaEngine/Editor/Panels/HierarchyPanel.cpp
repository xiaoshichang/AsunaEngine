
#include "HierarchyPanel.h"
#include <imgui.h>
#include "../Events/EditorEventType.h"
#include "../Events/EditorEventManager.h"
#include "Engine/Foundation/Logger/Logger.h"

using namespace  asuna;


void HierarchyPanel::Initialize()
{

}

void HierarchyPanel::Render()
{
    ImGui::Begin("Hierarchy Window", &m_Showing);
    RenderGameObject(SceneManager::Instance->GetRoot().get());
    ImGui::End();
}

void HierarchyPanel::RenderGameObject(GameObject* gameObject)
{
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if (gameObject == m_SelectedObject)
    {
        node_flags |= ImGuiTreeNodeFlags_Selected;
    }

    if (ImGui::TreeNodeEx(gameObject->GetName().c_str(), node_flags))
    {
        if (ImGui::IsItemClicked())
        {
            m_SelectedObject = gameObject;
            EditorEventManager::Dispatch(EditorEventType::OnHierarchPanelSelectNewObject, gameObject);
        }

        auto children = gameObject->GetTransform()->GetChildren();
        for(const auto& child : children)
        {
            RenderGameObject(child->GetOwner());
        }
        ImGui::TreePop();
    }
}

void HierarchyPanel::Finalize()
{

}


