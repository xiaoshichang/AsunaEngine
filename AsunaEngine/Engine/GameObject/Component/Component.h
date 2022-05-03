//
// Created by xiao on 2022/3/30.
//

#pragma once
#include <memory>

namespace asuna
{
    class IComponent
    {
    public:
        virtual void Initialize() = 0;
        virtual void Finalize() = 0;
        virtual void Tick() = 0;
    };


    class GameObject;
    class Component : public IComponent
    {
    public:

        explicit Component(GameObject* owner) : m_Owner(owner)
        {
        }

        virtual ~Component() = default;

        GameObject* GetOwner() { return m_Owner;}

    protected:
        GameObject* m_Owner;
    };

}

