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

void TransformCmpt::SetEuler(float x, float y, float z)
{
    m_Rotation = Quaternion::FromRollPitchYawLH(x, y, z);
}

void TransformCmpt::AddChild(TransformCmpt* child)
{
    if (std::find(m_Children.begin(), m_Children.end(), child) != m_Children.end())
    {
        ASUNA_ASSERT(false);
        return;
    }
    m_Children.push_back(child);
}

void TransformCmpt::RemoveChild(TransformCmpt* child)
{
    if (std::find(m_Children.begin(), m_Children.end(), child) == m_Children.end())
    {
        ASUNA_ASSERT(false);
        return;
    }
    auto item = std::remove(m_Children.begin(), m_Children.end(), child);
}
