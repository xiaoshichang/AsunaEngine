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
		void AddRenderItem(const RenderItem* item);
        void Clear();
	private:
		std::vector<RenderItem const*> m_RenderItems;
	};
}


