//
// Created by xiao on 2022/3/30.
//

#include "TransformCmpt.h"

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
    m_RTSMatrixDirty = v != m_Position;
    m_Position = v;
}

void TransformCmpt::SetScale(float x, float y, float z)
{
    Vector3f v(x, y, z);
    m_RTSMatrixDirty = v != m_Scale;
    m_Scale = v;
}

void TransformCmpt::SetEuler(float x, float y, float z)
{
    auto q = Quaternion::FromRollPitchYawLH(x, y, z);
    m_RTSMatrixDirty = q != m_Rotation;
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
    auto t = BuildMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
    auto s = BuildMatrixScale(m_Scale.x, m_Scale.y, m_Scale.z);
    m_RTSMatrix = s * t * r;
    m_RTSMatrixDirty = false;
}
