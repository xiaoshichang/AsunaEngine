//
// Created by xiao on 2022/4/1.
//
#include <imgui.h>
#include "Engine/GameObject/Component/TransformCmpt/TransformCmpt.h"
#include "TransformCmptEditor.h"

void asuna::TransformCmptEditor::Render(asuna::TransformCmpt *transform)
{
    auto position = transform->GetPosition();
    auto rotation = transform->GetEuler();
    auto scale = transform->GetScale();

    ImGui::Separator();
    ImGui::Text("TransformCmpt");
    ImGui::Text("Position"); ImGui::SameLine();
    ImGui::SetCursorPosX(80);
    ImGui::PushItemWidth(80);
    ImGui::PushID("px");
    ImGui::DragFloat("X", reinterpret_cast<float *>(&position.x), 0.1f); ImGui::SameLine();
    ImGui::PopID();
    ImGui::PushID("py");
    ImGui::DragFloat("Y", reinterpret_cast<float *>(&position.y), 0.1f); ImGui::SameLine();
    ImGui::PopID();
    ImGui::PushID("pz");
    ImGui::DragFloat("Z", reinterpret_cast<float *>(&position.z), 0.1f);
    ImGui::PopID();
    ImGui::PopItemWidth();

    //https://stackoverflow.com/questions/11165556/converting-yaw-euler-angles-in-range-90-90-to-0-360
    ImGui::Text("Rotation"); ImGui::SameLine();
    ImGui::SetCursorPosX(80);
    ImGui::PushItemWidth(80);
    ImGui::PushID("rx");
    ImGui::DragFloat("X", reinterpret_cast<float *>(&rotation.x), 0.05f, -PI/2, PI/2, "%.4f"); ImGui::SameLine();
    ImGui::PopID();
    ImGui::PushID("ry");
    ImGui::DragFloat("Y", reinterpret_cast<float *>(&rotation.y), 0.05f, -PI, PI, "%.4f"); ImGui::SameLine();
    ImGui::PopID();
    ImGui::PushID("rz");
    ImGui::DragFloat("Z", reinterpret_cast<float *>(&rotation.z), 0.05f, -PI, PI, "%.4f");
    ImGui::PopID();
    ImGui::PopItemWidth();

    ImGui::Text("Scale"); ImGui::SameLine();
    ImGui::SetCursorPosX(80);
    ImGui::PushItemWidth(80);
    ImGui::PushID("sx");
    ImGui::DragFloat("X", reinterpret_cast<float *>(&scale.x), 0.1f); ImGui::SameLine();
    ImGui::PopID();
    ImGui::PushID("sy");
    ImGui::DragFloat("Y", reinterpret_cast<float *>(&scale.y), 0.1f); ImGui::SameLine();
    ImGui::PopID();
    ImGui::PushID("sz");
    ImGui::DragFloat("Z", reinterpret_cast<float *>(&scale.z), 0.1f);
    ImGui::PopID();
    ImGui::PopItemWidth();

    if (position == transform->GetPosition())
    {
    }
    else
    {
        transform->SetPosition(position.x, position.y, position.z);
    }

    if (rotation.x >= PI/2 )
    {
        rotation.x = PI/2;
    }
    if (rotation.x <= -PI/2)
    {
        rotation.x = -PI/2;
    }
    if (rotation == transform->GetEuler())
    {
    }
    else
    {
        transform->SetEuler(rotation.x, rotation.y, rotation.z);
    }

    if (scale == transform->GetScale())
    {
    }
    else
    {
        transform->SetScale(scale.x, scale.y, scale.z);
    }

}
