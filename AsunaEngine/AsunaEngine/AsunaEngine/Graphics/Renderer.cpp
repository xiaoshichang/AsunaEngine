
#include <algorithm>
#include "Renderer.h"

using namespace asuna;

Renderer* Renderer::Current = nullptr;

void Renderer::Render()
{
	ClearRenderTarget(0.1f, 0.2f, 0.3f, 1.0f);
	m_RenderItemQueue->Render();
	Present();
}

void Renderer::SetRenderSurface(RenderSurface* surface)
{
	m_Surface = surface;
}

RenderContext* asuna::Renderer::GetContext()
{
	return m_Context;
}

void Renderer::AddRenderItem(RenderItem* item)
{
	m_RenderItemQueue->AddRenderItem(item);
}

void Renderer::RemoveRenderItem(RenderItem* item)
{
	m_RenderItemQueue->RemoveRenderItem(item);
}

RenderAPIType Renderer::GetRenderAPIType()
{
	return m_APIType;
}
