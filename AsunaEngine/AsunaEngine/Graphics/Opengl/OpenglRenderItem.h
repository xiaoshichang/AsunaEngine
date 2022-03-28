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
		OpenglRenderItem(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> vs, std::shared_ptr<Shader> ps, std::shared_ptr<ConstantBuffer> cb) :
			RenderItem(mesh, vs, ps, cb)
		{
		}

		virtual ~OpenglRenderItem()
		{
		}

		virtual void Render() override;

		static std::shared_ptr<OpenglRenderItem> Create(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> vs, std::shared_ptr<Shader> ps, std::shared_ptr<ConstantBuffer> cb);

	private:
		std::shared_ptr<OpenglShaderProgram> m_ShaderProgram;
		std::shared_ptr<OpenglMesh> m_Mesh;


	};
}

