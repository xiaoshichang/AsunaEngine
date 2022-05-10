//
// Created by xiao on 2022/4/21.
//
#pragma once
#include "../Component.h"
#include "../../../Foundation/Math/AMath.h"

namespace asuna
{
    enum class LightType
    {
        Direction,
        Spot,
        Point,
    };


    class LightCmpt : public Component
    {
    public:
        LightCmpt() = delete;
        explicit LightCmpt(GameObject* owner) :
            Component(owner),
            m_LightType(LightType::Direction),
            m_Intensity(1.0f),
            m_LightColor(1, 1, 1, 1)
        {
        }

        ~LightCmpt() override = default;

        void Initialize() override;
        void Finalize() override;
        void Tick() override;

        void SetColor(const Color& color) { m_LightColor = color;}
        Color& GetColor() {return m_LightColor;}
        void SetIntensity(float intensity) { m_Intensity = intensity;}
        float& GetIntensity() {return m_Intensity;}
        void SetLightType(LightType lt);
        LightType GetLightType() const {return m_LightType;}
        Matrix4x4f GetLightViewProjMatrix();

    private:
        Color m_LightColor;
        float m_Intensity;
        LightType m_LightType;



    };

}




