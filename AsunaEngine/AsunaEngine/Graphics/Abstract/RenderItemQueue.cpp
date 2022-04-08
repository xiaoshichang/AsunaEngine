#include "RenderItemQueue.h"

using namespace asuna;

void RenderItemQueue::Render()
{
    BindConstantBufferPerFrame();
	for (const auto& item : m_RenderItems)
	{
		item->Render();
	}
}

void RenderItemQueue::AddRenderItem(const std::shared_ptr<RenderItem>& item)
{
	m_RenderItems.push_back(item);
}

void RenderItemQueue::RemoveRenderItem(const std::shared_ptr<RenderItem>& item)
{
	m_RenderItems.remove(item);
}
