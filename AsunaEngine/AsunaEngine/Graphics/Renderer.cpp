
#include <algorithm>
#include "Renderer.h"

using namespace asuna;
using namespace std;

Renderer* Renderer::Current = nullptr;

void Renderer::RenderScene(std::shared_ptr<RenderTarget> rt)
{
	ClearRenderTarget(rt, 0.1f, 0.2f, 0.3f, 1.0f);
	SetRenderTarget(rt);
	m_RenderItemQueue->Render();
}

shared_ptr<RenderContext> asuna::Renderer::GetContext()
{
	return m_Context;
}

void Renderer::AddRenderItem(shared_ptr<RenderItem> item)
{
	m_RenderItemQueue->AddRenderItem(item);
}

void Renderer::RemoveRenderItem(shared_ptr<RenderItem> item)
{
	m_RenderItemQueue->RemoveRenderItem(item);
}

bool Renderer::CheckLeftHandRenderAPI() const
{
    switch (m_APIType)
    {
        case RenderAPIType::Directx11:
        case RenderAPIType::Directx12:
            return true;
        case RenderAPIType::Opengl:
        case RenderAPIType::Opengles:
            return false;
        default:
            ASUNA_ASSERT(false);
            return false;
    }
}