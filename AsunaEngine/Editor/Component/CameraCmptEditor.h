//
// Created by xiao on 2022/4/2.
//
#pragma once
#include <memory>
#include "Engine/GameObject/Component/CameraCmpt/CameraCmpt.h"
namespace asuna
{
    class CameraCmptEditor
    {
    public:
        static void Render(const std::shared_ptr<CameraCmpt>& camera);
    };


}




