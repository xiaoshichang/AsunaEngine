//
// Created by xiao on 2022/3/30.
//
#pragma once
#include <memory>
#include <string>
#include "Component/Transform/TransformCmpt.h"

namespace asuna
{
    class GameObject
    {
    public:
        GameObject() = delete;
        explicit GameObject(const std::string& name);
        ~GameObject();


    public:
        std::string GetName() { return m_Name;}
        void SetName(const std::string& name) {m_Name = name;}

        TransformCmpt* GetTransform() {return m_TransformCmpt;}


    private:
        std::string m_Name;
        TransformCmpt* m_TransformCmpt;

    };
}



