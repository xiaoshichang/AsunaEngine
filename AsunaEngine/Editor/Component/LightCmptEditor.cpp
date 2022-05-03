//
// Created by xiao on 2022/5/3.
//

#include "LightCmptEditor.h"
#include "Engine/GameObject/GameObject.h"
#include <imgui.h>

const char* LightTypeName[] = {"Direction", "Spot"};

void asuna::LightCmptEditor::Render(asuna::LightCmpt *light)
{
    if (light == nullptr)
    {
        return;
    }

    ImGui::Separator();
    ImGui::Text("LightCmpt");

    int lightType = (int)light->GetLightType();
    ImGui::Combo("Projection", &lightType, LightTypeName, IM_ARRAYSIZE(LightTypeName), IM_ARRAYSIZE(LightTypeName));

    if ((LightType)lightType == LightType::Direction)
    {
        // direction
        auto direction = light->GetOwner()->GetTransform()->GetForward();
        ImGui::Text("Direction: %f, %f, %f", direction.x, direction.y, direction.z);
    }

    Color& color = light->GetColor();
    ImGui::DragFloat3("Color", reinterpret_cast<float *>(&color), 0.01f, 0.0f, 1.0f);
    float& intensity = light->GetIntensity();
    ImGui::DragFloat("Intensity", reinterpret_cast<float *>(&intensity), 0.01, 0.0f, 10.0f);
    light->SetLightType((LightType)lightType);
}
