//
// Created by xiao on 2022/5/3.
//

#pragma once
#include <memory>
#include "Engine/GameObject/Component/LightCmpt/LightCmpt.h"

namespace asuna
{
    class LightCmptEditor
    {
    public:
        static void Render(const std::shared_ptr<LightCmpt>& light);
    };

}


