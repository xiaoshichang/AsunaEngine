#pragma once

#include "../Renderer.h"
#include "../../DLLExport/DLLExport.h"

namespace asuna
{
	class ASUNAENGINE_API OpenGLRenderer : public Renderer
	{

	public:
		virtual void Initialize();
		virtual void Finalize();

	private:
		virtual void ClearRenderTarget(float r, float g, float b, float a) override;
		virtual void Present() override;
		virtual void CreateDeviceContext() override;
		virtual void ReleaseDeviceContext() override;

	private:
		void LoadWGL();
		void CreateGLContext();

	private:
		bool InitializeBuffers();

	private:
		HGLRC m_RenderContext;
		HDC     m_hDC = 0;

		unsigned int g_vertexShader;
		unsigned int g_fragmentShader;
		unsigned int g_shaderProgram;
	};

}


