#pragma once
#include "../RenderItem.h"
#include "OpenglMesh.h"
#include "OpenglVertexBuffer.h"
#include "OpenglIndexBuffer.h"
#include "OpenglShader.h"

namespace asuna
{
	class OpenglRenderItem : public RenderItem
	{
	public:
		OpenglRenderItem(OpenglMesh* mesh,  OpenglShaderProgram* sp) :
			m_Mesh(mesh),
			m_ShaderProgram(sp)
		{}

		virtual void Render() override;

	private:
		OpenglShaderProgram* m_ShaderProgram;
		OpenglMesh* m_Mesh;


	};
}

