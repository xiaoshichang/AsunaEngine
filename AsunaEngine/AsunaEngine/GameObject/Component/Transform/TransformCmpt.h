//
// Created by xiao on 2022/3/30.
//

#pragma once
#include <vector>
#include "../Component.h"
#include "../../../Foundation/Math/AMath.h"


namespace asuna
{
    class TransformCmpt : public Component
    {
    public:
        TransformCmpt(GameObject* owner) :
            m_Position(0, 0, 0),
            m_Scale(1, 1, 1),
            m_Rotation(0, 0, 0, 1),
            Component(owner)
        {
        }

        void Initialize() override;
        void Finalize() override;
        void Tick() override;

        void SetParent(TransformCmpt* parent ) { m_Parent = parent;}
        TransformCmpt* GetParent() { return m_Parent;}
        void AddChild(TransformCmpt* child);
        void RemoveChild(TransformCmpt* child);
        std::vector<TransformCmpt*>& GetChildren() { return m_Children;}

        Vector3f GetPosition() {return m_Position;}
        void SetPosition(float x, float y, float z);
        Vector3f GetScale() {return m_Scale;}
        void SetScale(float x, float y, float z);
        Vector3f GetEuler();
        void SetEuler(float x, float y, float z);
        Matrix4x4f GetRTSMatrix();

    private:
        void CalculateRTSMatrix();

    private:
        Vector3f m_Position;
        Vector3f m_Scale;
        Quaternion m_Rotation;
        TransformCmpt* m_Parent;
        std::vector<TransformCmpt*> m_Children;
        bool m_RTSMatrixDirty = false;
        Matrix4x4f m_RTSMatrix;

    };
}



