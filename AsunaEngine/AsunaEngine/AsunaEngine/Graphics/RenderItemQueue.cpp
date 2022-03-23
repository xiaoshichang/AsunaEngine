#include "RenderItemQueue.h"

void asuna::RenderItemQueue::Render()
{
	for (auto item : m_RenderItems)
	{
		item->Render();
	}
}

void asuna::RenderItemQueue::AddRenderItem(RenderItem* item)
{
	m_RenderItems.push_back(item);
}

void asuna::RenderItemQueue::RemoveRenderItem(RenderItem* item)
{
	m_RenderItems.remove(item);
}
