//
// Created by xiao on 2022/4/5.
//

#include "DirectX11RenderItemQueue.h"
#include "DirectX11Renderer.h"
#include "../../Scene/SceneManager.h"

using namespace asuna;
using namespace std;

void asuna::DirectX11RenderItemQueue::BindConstantBufferPerFrame()
{
    SceneManager::Instance->GetConstantBufferPerScene()->Bind();
}

