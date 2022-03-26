
#include <algorithm>
#include "Renderer.h"

using namespace asuna;
using namespace std;

Renderer* Renderer::Current = nullptr;

void Renderer::Render()
{
	ClearRenderTarget(0.1f, 0.2f, 0.3f, 1.0f);
	m_RenderItemQueue->Render();
	Present();
}

void Renderer::SetRenderSurface(shared_ptr<RenderSurface> surface)
{
	m_Surface = surface;
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

RenderAPIType Renderer::GetRenderAPIType()
{
	return m_APIType;
}
