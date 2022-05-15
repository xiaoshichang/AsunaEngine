//
// Created by xiao on 2022/4/21.
//

#include "LightCmpt.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Graphics/Abstract/Renderer.h"

using namespace asuna;

void LightCmpt::Initialize()
{
    SceneManager::Instance->RegisterLight(this);
}

void LightCmpt::Finalize()
{
    SceneManager::Instance->UnregisterLight(this);
}

void LightCmpt::Tick()
{

}

void LightCmpt::SetLightType(LightType lt)
{
    if (lt == LightType::Spot || lt == LightType::Point)
    {
        Logger::Error("Unsupported light type!");
        return;
    }
    m_LightType = lt;
}

Matrix4x4f LightCmpt::GetLightViewProjMatrix()
{
    Vector3f eye = GetOwner()->GetTransform()->GetPosition();
    Vector3f up(0, 1, 0);
    Vector3f direction = GetOwner()->GetTransform()->GetForward();
    Vector3f focus = eye + direction;

    Matrix4x4f ret;
    if (m_LightType == LightType::Direction)
    {
        // todo: how to correctly set these parameters?
        auto view = BuildMatrixViewLookatLH(eye, focus, up);
        auto proj = BuildMatrixOrthographicDX(64, 64, 10, 50);
        ret = proj * view;
    }
    else
    {
        ASUNA_ASSERT(false);
    }
    return ret;
}
