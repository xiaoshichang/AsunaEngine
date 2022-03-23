#pragma once
#include "../RenderItem.h"
#include "OpenglVertexArray.h"
#include "OpenglVertexBuffer.h"
#include "OpenglIndexBuffer.h"
#include "OpenglShader.h"

namespace asuna
{
	class OpenglRenderItem : public RenderItem
	{
	public:
		OpenglRenderItem(OpenglVertexArray* va,  OpenglShaderProgram* sp) :
			m_VertexArray(va),
			m_ShaderProgram(sp)
		{}

		virtual void Render() override;

	private:
		OpenglShaderProgram* m_ShaderProgram;
		OpenglVertexArray* m_VertexArray;


	};
}

