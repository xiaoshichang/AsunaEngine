//
// Created by xiao on 2022/4/12.
//

#include "OpenglRenderItemQueue.h"
#include "../../Scene/SceneManager.h"

void asuna::OpenglRenderItemQueue::BindConstantBufferPerFrame()
{
    SceneManager::Instance->GetConstantBufferPerScene()->Bind();
}
