#pragma once
#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "Shader.h"
#include "RenderItem.h"
#include "ConstantBuffer.h"

namespace asuna
{
	class RenderItem
	{
	public:
		RenderItem() = delete;
		RenderItem(
			std::shared_ptr<Mesh> mesh,
			std::shared_ptr<Shader> vertexShader,
			std::shared_ptr<Shader> pixelShader,
			std::shared_ptr<ConstantBuffer> constantBuffer):
			m_Mesh(mesh),
			m_VertexShader(vertexShader),
			m_PixelShader(pixelShader),
			m_ConstantBuffer(constantBuffer)
		{}

		virtual ~RenderItem()
		{
		}

		std::shared_ptr<Mesh> GetMesh()
		{
			return m_Mesh;
		}

		std::shared_ptr<Shader> GetVertexShader()
		{
			return m_VertexShader;
		}

		std::shared_ptr<Shader> GetPixelShader()
		{
			return m_PixelShader;
		}

		std::shared_ptr<ConstantBuffer> GetConstantBuffer()
		{
			return m_ConstantBuffer;
		}


	public:
		virtual void Render() = 0;


	private:
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<Shader> m_VertexShader;
		std::shared_ptr<Shader> m_PixelShader;
		std::shared_ptr<ConstantBuffer> m_ConstantBuffer;
	};
}


