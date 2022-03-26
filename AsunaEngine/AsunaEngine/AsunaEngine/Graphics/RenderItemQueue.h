#pragma once
#include <list>

#include <memory>
#include "RenderItem.h"

namespace asuna
{
	class RenderItemQueue
	{
	public:
		virtual ~RenderItemQueue()
		{
		}

	public:
		void Render();
		void AddRenderItem(std::shared_ptr<RenderItem>);
		void RemoveRenderItem(std::shared_ptr<RenderItem>);
	private:
		std::list<std::shared_ptr<RenderItem>> m_RenderItems;
	};
}


