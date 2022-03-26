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
		OpenglRenderItem(std::shared_ptr<OpenglMesh> mesh, std::shared_ptr<OpenglShaderProgram> sp) :
			m_Mesh(mesh),
			m_ShaderProgram(sp)
		{
		}

		virtual ~OpenglRenderItem()
		{
		}

		virtual void Render() override;

	private:
		std::shared_ptr<OpenglShaderProgram> m_ShaderProgram;
		std::shared_ptr<OpenglMesh> m_Mesh;


	};
}

