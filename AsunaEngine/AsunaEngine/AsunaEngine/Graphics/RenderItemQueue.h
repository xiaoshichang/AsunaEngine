#pragma once
#include <list>

#include "RenderItem.h"
namespace asuna
{
	class RenderItemQueue
	{
	public:
		void Render();
		void AddRenderItem(RenderItem* item);
		void RemoveRenderItem(RenderItem* item);
	private:
		std::list<RenderItem*> m_RenderItems;
	};
}


