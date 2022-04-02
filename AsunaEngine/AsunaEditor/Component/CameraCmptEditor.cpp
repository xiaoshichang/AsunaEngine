//
// Created by xiao on 2022/4/2.
//
#include <imgui.h>
#include "CameraCmptEditor.h"

void asuna::CameraCmptEditor::Render(asuna::CameraCmpt *camera)
{
    static int selectedType = 0;
    float& far = camera->GetFar();
    float& near = camera->GetNear();

    const char* CameraProjectionType[] = {"Perspective", "Orthographic"};

    ImGui::Separator();
    ImGui::Text("Camera");
    ImGui::Combo("Projection", &selectedType, CameraProjectionType, IM_ARRAYSIZE(CameraProjectionType), IM_ARRAYSIZE(CameraProjectionType));
    if (selectedType == 0)
    {
        float& fov = camera->GetFov();
        float& respectRatio = camera->GetAspectRatio();
        ImGui::DragFloat("Fov", &fov, 0.001, 0.001, PI/2);
        ImGui::DragFloat("Respect Ratio", &respectRatio, 0.001, 0.001, 10);
    }
    else if (selectedType == 1)
    {
        float& viewWidth = camera->GetViewWidth();
        float& viewHeight = camera->GetViewHeight();
        ImGui::DragFloat("Width", &viewWidth, 1, 1, 100000);
        ImGui::DragFloat("Height", &viewHeight, 1, 1, 100000);
    }
    else
    {
    }
    ImGui::DragFloat("Far", &far, 1, 1, 100000);
    ImGui::DragFloat("Near", &near, 1, 0.01, 100000);
}
