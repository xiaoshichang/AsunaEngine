//
// Created by xiao on 2022/4/2.
//
#include <imgui.h>
#include "CameraCmptEditor.h"
using namespace asuna;
using namespace std;

void asuna::CameraCmptEditor::Render(const shared_ptr<CameraCmpt>& camera)
{
    if (camera == nullptr)
    {
        return;
    }

    int selectedType = (int)camera->GetProjectionType();
    float far = camera->GetFar();
    float near = camera->GetNear();

    const char* CameraProjectionType[] = {"perspective", "orthographic"};

    ImGui::Separator();
    ImGui::Text("camera");
    ImGui::Combo("projection", &selectedType, CameraProjectionType, IM_ARRAYSIZE(CameraProjectionType), IM_ARRAYSIZE(CameraProjectionType));
    if (selectedType == 0)
    {
        float fov = camera->GetFov();
        float aspectRatio = camera->GetAspectRatio();
        ImGui::DragFloat("fov", &fov, 0.001, 0.001, PI/2);
        ImGui::DragFloat("aspect ratio", &aspectRatio, 0.001, 0.001, 10);
        camera->SetFov(fov);
        camera->SetAspectRatio(aspectRatio);
        camera->SetProjectionType(CameraProjectionType::Perspective);
    }
    else if (selectedType == 1)
    {
        float viewWidth = camera->GetViewWidth();
        float viewHeight = camera->GetViewHeight();
        ImGui::DragFloat("Width", &viewWidth, 1, 1, 100000);
        ImGui::DragFloat("Height", &viewHeight, 1, 1, 100000);
        camera->SetViewWidth(viewWidth);
        camera->SetViewHeight(viewHeight);
        camera->SetProjectionType(CameraProjectionType::Orthographic);
    }
    else
    {
    }
    ImGui::DragFloat("Far", &far, 1, 1, 100000);
    ImGui::DragFloat("Near", &near, 0.01, 0.01, 100000);

    camera->SetFar(far);
    camera->SetNear(near);
    if (far <= near)
    {
        ImGui::Text("Far mush larger then Near!");
    }
}
