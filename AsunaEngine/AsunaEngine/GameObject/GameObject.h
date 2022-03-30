//
// Created by xiao on 2022/3/30.
//
#pragma once
#include <string>
#include "Component/Transform/TransformCmpt.h"

namespace asuna
{
    class GameObject
    {
    public:
        GameObject() = delete;
        explicit GameObject(const std::string& name);





    private:
        const std::string m_Name;
        TransformCmpt m_TransformCmpt;

    };
}



