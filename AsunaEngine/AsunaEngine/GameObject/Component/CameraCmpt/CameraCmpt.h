//
// Created by xiao on 2022/4/2.
//
#pragma once
#include "../Component.h"
#include "../../../Foundation/Math/AMath.h"

namespace asuna
{
    enum class CameraProjectionType
    {
        Perspective = 1,
        Orthographic
    };


    class CameraCmpt : public Component
    {
    public:
        CameraCmpt() = delete;
        CameraCmpt(GameObject* owner) :
            m_ProjectionType(CameraProjectionType::Orthographic),
            m_Near(0.1f),
            m_Far(1000.0f),
            m_Fov(PI / 4.0f),
            m_AspectRatio(4.0f/3),
            m_ViewWidth(1200),
            m_ViewHeight(800),
            Component(owner)
        {
        }

        void Initialize() override;
        void Finalize() override;

        float& GetNear()
        {
            return m_Near;
        }

        void SetNear(float nearPlane)
        {
            m_Near = nearPlane;
        }

        float& GetFar()
        {
            return m_Far;
        }

        void SetFar(float farPlane)
        {
            m_Far = farPlane;
        }

        float& GetFov()
        {
            return m_Fov;
        }

        void SetFov(float fov)
        {
            m_Fov = fov;
        }

        float& GetAspectRatio()
        {
            return m_AspectRatio;
        }

        void SetAspectRatio(float ar)
        {
            m_AspectRatio = ar;
        }

        float& GetViewWidth()
        {
            return m_ViewWidth;
        }

        void SetViewWidth(float vw)
        {
            m_ViewWidth = vw;
        }

        float& GetViewHeight()
        {
            return m_ViewHeight;
        }

        void SetViewHeight(float vh)
        {
            m_ViewHeight = vh;
        }


    private:
        CameraProjectionType m_ProjectionType;
        float m_Near;
        float m_Far;
        float m_Fov;
        float m_AspectRatio;
        float m_ViewWidth;
        float m_ViewHeight;
    };
}

