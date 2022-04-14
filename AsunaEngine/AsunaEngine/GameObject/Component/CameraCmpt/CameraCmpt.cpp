//
// Created by xiao on 2022/4/2.
//

#include "CameraCmpt.h"
#include "../../../Graphics/Abstract/Renderer.h"
#include "../../../Scene/SceneManager.h"

using namespace asuna;

void CameraCmpt::Initialize()
{
    SceneManager::Instance->AddCamera(this);
}

void CameraCmpt::Finalize()
{
    SceneManager::Instance->RemoveCamera(this);
}

void CameraCmpt::Tick()
{

}

const Matrix4x4f &CameraCmpt::GetProjectionMatrix()
{
    if (m_ProjectionMatrixDirty)
    {
        CalculateProjectionMatrix();
    }
    return m_ProjectionMatrix;
}

void CameraCmpt::CalculateProjectionMatrix()
{
    if (Renderer::Current->CheckLeftHandRenderAPI())
    {
        if (m_ProjectionType == CameraProjectionType::Perspective)
        {
            m_ProjectionMatrix = BuildMatrixPerspectiveFovLH(m_Fov, m_AspectRatio, m_Near, m_Far);
        }
        else
        {
            m_ProjectionMatrix = BuildMatrixOrthographicLH(m_ViewWidth, m_ViewHeight, m_Near, m_Far);
        }
    }
    else
    {
        if (m_ProjectionType == CameraProjectionType::Perspective)
        {
            m_ProjectionMatrix = BuildMatrixPerspectiveFovRH(m_Fov, m_AspectRatio, m_Near, m_Far);
        }
        else
        {
            m_ProjectionMatrix = BuildMatrixOrthographicRH(m_ViewWidth, m_ViewHeight, m_Near, m_Far);
        }
    }
    m_ProjectionMatrixDirty = false;
}

const Matrix4x4f &CameraCmpt::GetViewMatrix() 
{
    auto transform = GetOwner()->GetTransform();
    auto eye = transform->GetPosition();
    // todo: get correct up direction by rotation
    Vector3f up(0, 1, 0);
    Vector3f look(0, 0, 0);
    if (Renderer::Current->CheckLeftHandRenderAPI())
    {
        m_ViewMatrix = BuildMatrixViewLookatLH(eye, look, up);
    }
    else
    {
        Vector3f rightHandEye(eye.x, eye.y, -eye.z);
        m_ViewMatrix = BuildMatrixViewLookatRH(rightHandEye, look, up);
    }
    return m_ViewMatrix;
}






