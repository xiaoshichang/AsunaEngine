//
// Created by xiao on 2022/4/2.
//

#include "CameraCmpt.h"
#include "../../../Graphics/Abstract/Renderer.h"
#include "../../../Scene/SceneManager.h"

using namespace asuna;

void CameraCmpt::Initialize()
{
    SceneManager::Instance->RegisterCamera(this);
}

void CameraCmpt::Finalize()
{
    SceneManager::Instance->UnregisterCamera(this);
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
    //https://stackoverflow.com/questions/47856866/opengl-glmortho-glm-coordinate-system-weird
    //https://stackoverflow.com/questions/21841598/when-does-the-transition-from-clip-space-to-screen-coordinates-happen
    if (m_ProjectionType == CameraProjectionType::Perspective)
    {
        m_ProjectionMatrix = BuildMatrixPerspectiveFovDX(m_Fov, m_AspectRatio, m_Near, m_Far);
    }
    else
    {
        m_ProjectionMatrix = BuildMatrixOrthographicDX(m_ViewWidth, m_ViewHeight, m_Near, m_Far);
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
    m_ViewMatrix = BuildMatrixViewLookatLH(eye, look, up);
    return m_ViewMatrix;
}






