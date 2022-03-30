//
// Created by xiao on 2022/3/30.
//

#pragma once
#include "../Component.h"
#include "../../../Foundation/Math/AMath.h"


namespace asuna
{
    class TransformCmpt : public Component
    {
    public:
        TransformCmpt();
        void Initialize() override;
        void Finalize() override;

    private:
        Vector3f m_Position;
        Vector3f m_Scale;
        Quaternion m_Rotation;

    };
}



