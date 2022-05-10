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
    Vector3f eye(0, 20, 0);
    Vector3f up(0, 1, 0);
    Vector3f direction = GetOwner()->GetTransform()->GetForward();
    Vector3f focus = eye + direction;

    Matrix4x4f ret;
    if (m_LightType == LightType::Direction)
    {
        if (Renderer::Instance->CheckLeftHandRenderAPI())
        {
            // todo: how to correctly set these parameters?
            auto proj = BuildMatrixOrthographicLH(50, 50, 1, 50);
            auto view = BuildMatrixViewLookatLH(eye, focus, up);
            ret = proj * view;
        }
        else
        {
            auto proj = BuildMatrixOrthographicRH(50, 50, 1, 50);
            Vector3f rightEye(eye.x, eye.y, -eye.z);
            Vector3f rightFocus(focus.x, focus.y, -focus.z);
            auto view = BuildMatrixViewLookatRH(rightEye, rightFocus, up);
            ret = proj * view;
        }
    }
    else
    {
        Logger::Error("Unsupported light type for shadow mapping");
        ASUNA_ASSERT(false);
    }
    return ret;
}
