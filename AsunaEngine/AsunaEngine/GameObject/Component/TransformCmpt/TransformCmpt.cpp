//
// Created by xiao on 2022/3/30.
//

#include "TransformCmpt.h"
#include "../../../Graphics/Abstract/Renderer.h"
using namespace asuna;


void TransformCmpt::Initialize()
{

}

void TransformCmpt::Finalize()
{

}

Vector3f TransformCmpt::GetEuler()
{
    return m_Rotation.ToEuler();
}

void TransformCmpt::AddChild(TransformCmpt* child)
{
    if (std::find(m_Children.begin(), m_Children.end(), child) != m_Children.end())
    {
        ASUNA_ASSERT(false);
        return;
    }
    m_Children.push_back(child);
    child->SetParent(this);
}

void TransformCmpt::RemoveChild(TransformCmpt* child)
{
    child->SetParent(nullptr);
    for(auto it = m_Children.begin(); it != m_Children.end(); it++)
    {
        if (*it == child)
        {
            m_Children.erase(it);
            return;
        }
    }
}

void TransformCmpt::Tick()
{

}

void TransformCmpt::SetPosition(float x, float y, float z)
{
    Vector3f v(x, y, z);
    m_RTSMatrixDirty = true;
    m_Position = v;
}

void TransformCmpt::SetScale(float x, float y, float z)
{
    Vector3f v(x, y, z);
    m_RTSMatrixDirty = true;
    m_Scale = v;
}

void TransformCmpt::SetEuler(float x, float y, float z)
{
    auto q = Quaternion::FromRollPitchYawLH(x, y, z);
    m_RTSMatrixDirty = true;
    m_Rotation = q;
}

Matrix4x4f TransformCmpt::GetRTSMatrix()
{
    if (m_RTSMatrixDirty)
    {
        CalculateRTSMatrix();
    }
    return m_RTSMatrix;
}

void TransformCmpt::CalculateRTSMatrix()
{
    auto r = BuildMatrixQuaternion(m_Rotation);
    auto s = BuildMatrixScale(m_Scale.x, m_Scale.y, m_Scale.z);
    // todo: more standard asset pipeline
    if (Renderer::Current->CheckLeftHandRenderAPI())
    {
        auto assetDefault1 = BuildMatrixRotationX(PI / 2);
        auto t = BuildMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
        m_RTSMatrix = s * t * r * assetDefault1;
    }
    else
    {
        auto assetDefault1 = BuildMatrixRotationX(-PI / 2);
        auto t = BuildMatrixTranslation(m_Position.x, m_Position.y, -m_Position.z);
        m_RTSMatrix = s * t * r * assetDefault1;
    }
    m_RTSMatrixDirty = false;
}
