//
// Created by xiao on 2022/5/7.
//

#pragma once
#include "../RenderItemQueue.h"

namespace asuna
{
    class RenderPassMgr
    {
    public:
        RenderPassMgr();
        static RenderPassMgr* Instance;

    public:
        void RenderShadowMap(RenderItemQueue* queue);
        void RenderColor(RenderItemQueue* queue);

    private:


    };
}




