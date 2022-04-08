//
// Created by xiao on 2022/4/5.
//

#include "DirectX11RenderItemQueue.h"
#include "DirectX11Renderer.h"
#include "DirectX11RenderContext.h"
#include "DirectX11ConstantBuffer.h"
#include <memory>
#include "../../Scene/SceneManager.h"

using namespace asuna;
using namespace std;

void asuna::DirectX11RenderItemQueue::BindConstantBufferPerFrame()
{
    auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Current->GetContext()).get();
    auto cb = dynamic_pointer_cast<DirectX11ConstantBuffer>(SceneManager::Instance->GetConstantBufferPerScene());
    auto data = (ConstantBufferDataPerFrame*)cb->GetData();
    auto buffer = cb->GetBuffer();
    // map data
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    auto result = context->m_DeviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    ASUNA_ASSERT(SUCCEEDED(result));
    auto dataPtr = (ConstantBufferDataPerFrame*)mappedResource.pData;
    memcpy(dataPtr, data, sizeof(ConstantBufferDataPerFrame));
    context->m_DeviceContext->Unmap(buffer, 0);
    context->m_DeviceContext->VSSetConstantBuffers(0, 1, &buffer);
    context->m_DeviceContext->PSSetConstantBuffers(0, 1, &buffer);
}

