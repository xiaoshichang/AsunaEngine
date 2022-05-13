//
// Created by xiao on 2022/4/4.
//
#include <imgui.h>
#include "MeshRenderEditor.h"

using namespace asuna;

void MeshRenderEditor::Render(const std::shared_ptr<MeshRenderCmpt>& meshRender)
{
    if (meshRender == nullptr)
    {
        return;
    }

    ImGui::Separator();
    ImGui::Text("MeshRenderCmpt");
    ImGui::Text("Materials");
    for (int i = 0; i < meshRender->GetMaterialCount(); ++i)
    {
        auto material = meshRender->GetMaterial(i);
        auto& name = material->GetName();
        ImGui::Text("%d : %s", i, name.c_str());
    }

}
