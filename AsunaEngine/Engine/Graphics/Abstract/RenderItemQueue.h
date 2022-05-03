#pragma once
#include <list>

#include <memory>
#include "RenderItem.h"

namespace asuna
{
	class RenderItemQueue
	{
	public:
		virtual ~RenderItemQueue() = default;

	public:
		void Render();
        virtual void BindConstantBufferPerFrame() = 0;
		void AddRenderItem(const std::shared_ptr<RenderItem>&);
		void RemoveRenderItem(const std::shared_ptr<RenderItem>&);
	private:
		std::list<std::shared_ptr<RenderItem>> m_RenderItems;
	};
}


