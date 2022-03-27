#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace asuna
{
	class RenderItem
	{
	public:
		virtual ~RenderItem()
		{
		}

	public:
		virtual void Render() = 0;
	};
}


