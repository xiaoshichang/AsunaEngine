//
// Created by xiao on 2022/4/4.
//

#pragma once
#include <memory>
#include "Engine/GameObject/Component/MeshRenderCmpt/MeshRenderCmpt.h"

namespace asuna
{
    class MeshRenderEditor
    {
    public:
        static void Render(const std::shared_ptr<MeshRenderCmpt>& meshRender);
    };
}

