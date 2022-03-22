

#include "Renderer.h"

using namespace asuna;

Renderer* Renderer::Current = nullptr;

void asuna::Renderer::SetRenderSurface(RenderSurface* surface)
{
	m_Surface = surface;
}
