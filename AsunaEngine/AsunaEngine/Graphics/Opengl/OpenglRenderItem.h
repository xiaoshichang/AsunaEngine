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
		OpenglRenderItem(const std::shared_ptr<Mesh>& mesh, const std::vector<std::shared_ptr<Material>>& materials, const std::shared_ptr<ConstantBuffer>& perObject) :
			RenderItem(mesh, materials, perObject)
		{
		}

		virtual ~OpenglRenderItem()
		{
		}

		virtual void Render() override;

		static std::shared_ptr<OpenglRenderItem> Create(const std::shared_ptr<Mesh>& mesh, const std::vector<std::shared_ptr<Material>>& materials, const std::shared_ptr<ConstantBuffer>& perObject);

	private:
		std::shared_ptr<OpenglShaderProgram> m_ShaderProgram;
		std::shared_ptr<OpenglMesh> m_Mesh;


	};
}

