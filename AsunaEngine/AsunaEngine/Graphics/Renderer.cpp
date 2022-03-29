
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
