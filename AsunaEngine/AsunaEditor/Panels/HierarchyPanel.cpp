
#include "HierarchyPanel.h"
#include <imgui.h>
#include "AsunaEngine/Foundation/Logger/Logger.h"

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
            Logger::Info("click %s", gameObject->GetName().c_str());
            m_SelectedObject = gameObject;
        }

        auto children = gameObject->GetTransform()->GetChildren();
        for(const auto& child : children)
        {
            RenderGameObject(child->GetOwner());
        }
        ImGui::TreePop();
    }
}


