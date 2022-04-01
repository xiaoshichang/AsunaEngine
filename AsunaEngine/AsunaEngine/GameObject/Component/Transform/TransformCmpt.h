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

        void SetParent(TransformCmpt* parent ) { m_Parent = parent;}
        TransformCmpt* GetParent() { return m_Parent;}
        void AddChild(TransformCmpt* child);
        void RemoveChild(TransformCmpt* child);
        std::vector<TransformCmpt*>& GetChildren() { return m_Children;}

        Vector3f GetPosition() {return m_Position;}
        void SetPosition(float x, float y, float z) {m_Position.x = x; m_Position.y = y; m_Position.z = z;}
        Vector3f GetScale() {return m_Scale;}
        void SetScale(float x, float y, float z) {m_Scale.x = x; m_Scale.y = y; m_Scale.z = z;}
        Vector3f GetEuler();
        void SetEuler(float x, float y, float z);

    private:
        Vector3f m_Position;
        Vector3f m_Scale;
        Quaternion m_Rotation;
        TransformCmpt* m_Parent;
        std::vector<TransformCmpt*> m_Children;

    };
}



