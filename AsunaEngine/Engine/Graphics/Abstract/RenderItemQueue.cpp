#include "RenderItemQueue.h"

using namespace asuna;

void RenderItemQueue::Render()
{
    BindConstantBufferPerFrame();
	for (RenderItem const *item : m_RenderItems)
	{
		item->Render();
	}
}

void RenderItemQueue::AddRenderItem(const RenderItem* item)
{
	m_RenderItems.push_back(item);
}

void RenderItemQueue::Clear()
{
    m_RenderItems.clear();
}
