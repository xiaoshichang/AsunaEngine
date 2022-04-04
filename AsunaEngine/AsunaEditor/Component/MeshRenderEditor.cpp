//
// Created by xiao on 2022/4/4.
//
#include <imgui.h>
#include "MeshRenderEditor.h"

using namespace asuna;

void MeshRenderEditor::Render(asuna::MeshRenderCmpt *meshRender)
{
    if (meshRender == nullptr)
    {
        return;
    }

    ImGui::Separator();
    ImGui::Text("MeshRenderCmpt");
    ImGui::Text("Mesh: %s", meshRender->GetMeshPath().c_str());
    ImGui::Text("VS: %s", meshRender->GetVSPath().c_str());
    ImGui::Text("PS: %s", meshRender->GetPSPath().c_str());

}
