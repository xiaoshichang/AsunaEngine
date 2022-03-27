#pragma once

#include "../Renderer.h"
#include "../../DLLExport/DLLExport.h"

namespace asuna
{
	class ASUNAENGINE_API OpenGLRenderer : public Renderer
	{
	public:
		OpenGLRenderer()
		{
		}

		virtual ~OpenGLRenderer()
		{
		}

	public:
		virtual void Initialize(CreateRendererContextParam param);
		virtual void Finalize();
		virtual void ResizeResolution(int width, int height);
		virtual void Present() override;

	private:
		virtual void ClearRenderTarget(float r, float g, float b, float a) override;
		virtual void CreateDeviceContext() override;
		virtual void ReleaseDeviceContext() override;

	private:
		void LoadWGL();
		void CreateGLContext();
		void SetViewPort(int width, int height);


	private:
		bool InitializeBuffers();

	private:
		HGLRC m_RenderContext = 0;
		HDC   m_hDC = 0;

		unsigned int g_vertexShader = 0;
		unsigned int g_fragmentShader = 0;
		unsigned int g_shaderProgram = 0;
	};

}


