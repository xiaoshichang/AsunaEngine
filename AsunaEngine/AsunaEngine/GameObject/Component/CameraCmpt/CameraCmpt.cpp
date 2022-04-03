//
// Created by xiao on 2022/4/2.
//

#include "CameraCmpt.h"
#include "../../../Graphics/Renderer.h"
#include "../../../Foundation/Math/AMath.h"

void asuna::CameraCmpt::Initialize()
{

}

void asuna::CameraCmpt::Finalize()
{

}

void asuna::CameraCmpt::Tick()
{

}

const asuna::Matrix4x4f &asuna::CameraCmpt::GetProjectionMatrix()
{
    if (m_ProjectionMatrixDirty)
    {
        CalculateProjectionMatrix();
    }
    return m_ProjectionMatrix;
}

void asuna::CameraCmpt::CalculateProjectionMatrix()
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


