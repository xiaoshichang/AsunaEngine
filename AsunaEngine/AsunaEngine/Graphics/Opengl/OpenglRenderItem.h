#pragma once
#include "../Abstract/RenderItem.h"
#include "OpenglMesh.h"
#include "OpenglVertexBuffer.h"
#include "OpenglIndexBuffer.h"
#include "OpenglShader.h"

namespace asuna
{
	class OpenglRenderItem : public RenderItem
	{
	public:
		OpenglRenderItem(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> vs, std::shared_ptr<Shader> ps, std::shared_ptr<ConstantBuffer> perObject) :
			RenderItem(mesh, vs, ps, perObject)
		{
		}

		virtual ~OpenglRenderItem()
		{
		}

		virtual void Render() override;

		static std::shared_ptr<OpenglRenderItem> Create(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> vs, std::shared_ptr<Shader> ps, std::shared_ptr<ConstantBuffer> perObject);

	private:
		std::shared_ptr<OpenglShaderProgram> m_ShaderProgram;
		std::shared_ptr<OpenglMesh> m_Mesh;


	};
}

