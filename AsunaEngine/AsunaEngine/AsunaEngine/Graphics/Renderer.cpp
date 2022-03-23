
#include <algorithm>
#include "Renderer.h"

using namespace asuna;

Renderer* Renderer::Current = nullptr;

void asuna::Renderer::Render()
{
	ClearRenderTarget(0.1f, 0.2f, 0.3f, 1.0f);
	m_RenderItemQueue->Render();
	Present();
}

void asuna::Renderer::SetRenderSurface(RenderSurface* surface)
{
	m_Surface = surface;
}

RenderContext* asuna::Renderer::GetContext()
{
	return m_Context;
}

void asuna::Renderer::AddRenderItem(RenderItem* item)
{
	m_RenderItemQueue->AddRenderItem(item);
}

void asuna::Renderer::RemoveRenderItem(RenderItem* item)
{
	m_RenderItemQueue->RemoveRenderItem(item);
}
