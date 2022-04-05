#include <cstdint>
#include <fstream>
#include "OpenGLRenderer.h"
#include "OpenglMesh.h"
#include "OpenglRenderItem.h"
#include "../../3rd/Glad/include/glad/glad.h"
#include "../../3rd/Glad/include/glad/glad_wgl.h"
#include "../../Foundation/Platform/Assert.h"
#include "../../Foundation/Math/AMath.h"
#include "../../AssetLoader/AssetLoader.h"

using namespace std;
using namespace asuna;

#pragma comment(lib, "opengl32.lib")

static LRESULT CALLBACK TmpWndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}

	return 0;
}


void TransformCoord(asuna::Vector3f& vector, float* matrix)
{
	float x, y, z;
	// Transform the vector by the 3x3 matrix.
	x = (vector.x * matrix[0]) + (vector.y * matrix[3]) + (vector.z * matrix[6]);
	y = (vector.x * matrix[1]) + (vector.y * matrix[4]) + (vector.z * matrix[7]);
	z = (vector.x * matrix[2]) + (vector.y * matrix[5]) + (vector.z * matrix[8]);

	// Store the result in the reference.
	vector.x = x;
	vector.y = y;
	vector.z = z;

	return;
}


void asuna::OpenGLRenderer::Initialize(CreateRendererContextParam param)
{
	m_APIType = RenderAPIType::Opengl;
	m_ResolutionWidth = param.m_ResolutionWith;
	m_ResolutionHeight = param.m_ResolutionHeight;
	m_HWND = param.m_HWND;

	CreateDeviceContext();
}

void asuna::OpenGLRenderer::Finalize()
{
}

void asuna::OpenGLRenderer::ResizeResolution(int width, int height)
{
	glViewport(0, 0, width, height);
}

void asuna::OpenGLRenderer::ClearRenderTarget(std::shared_ptr<RenderTarget> rt, float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void asuna::OpenGLRenderer::Present()
{
	// Present the back buffer to the screen since rendering is complete.
	SwapBuffers(m_hDC);
}

void asuna::OpenGLRenderer::CreateDeviceContext()
{
	LoadWGL();
	CreateGLContext();
	int result;
	result = gladLoadGL();
	if (GLAD_GL_VERSION_3_0) {
		// Enable depth testing.
		glClearDepth(1.0f);
		// Enable depth testing.
		glEnable(GL_DEPTH_TEST);
	}
	glClearDepth(1.0f);

	// Enable depth testing.
	glEnable(GL_DEPTH_TEST);

	// Set the polygon winding to front facing for the left handed system.
	glFrontFace(GL_CCW);

	// Enable back face culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	SetViewPort(m_ResolutionWidth, m_ResolutionHeight);
	wglSwapIntervalEXT(0); // disable vertical sync to get high fps.
}

void asuna::OpenGLRenderer::ReleaseDeviceContext()
{
	if (m_RenderContext != 0)
	{
		wglDeleteContext(m_RenderContext);
		m_RenderContext = 0;
	}

	m_hDC = 0;

}



void asuna::OpenGLRenderer::LoadWGL()
{
	int result = 0;
	DWORD Style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	WNDCLASSEX WndClassEx;
	memset(&WndClassEx, 0, sizeof(WNDCLASSEX));
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WndClassEx.lpfnWndProc = TmpWndProc;
	WndClassEx.hInstance = hInstance;
	WndClassEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClassEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	WndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClassEx.lpszClassName = "InitWindow";
	RegisterClassEx(&WndClassEx);
	HWND TemphWnd = CreateWindowEx(WS_EX_APPWINDOW, WndClassEx.lpszClassName, "InitWindow", Style, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	HDC TemphDC = GetDC(TemphWnd);
	// Set a temporary default pixel format.
	int nPixelFormat = ChoosePixelFormat(TemphDC, &pfd);
	(nPixelFormat != 0);

	result = SetPixelFormat(TemphDC, nPixelFormat, &pfd);
	ASUNA_ASSERT(result == 1);

	// Create a temporary rendering context.
	m_RenderContext = wglCreateContext(TemphDC);
	ASUNA_ASSERT(result == 1);

	// Set the temporary rendering context as the current rendering context for this window.
	result = wglMakeCurrent(TemphDC, m_RenderContext);
	ASUNA_ASSERT(result == 1);

	auto r = gladLoadWGL(TemphDC);
	ASUNA_ASSERT(r != 0);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_RenderContext);
	ReleaseDC(TemphWnd, TemphDC);
	DestroyWindow(TemphWnd);
}

void asuna::OpenGLRenderer::CreateGLContext()
{
	int nPixelFormat;
	UINT numFormats;
	PIXELFORMATDESCRIPTOR pfd;
	int result;

	m_hDC = GetDC(m_HWND);

	if (GLAD_WGL_ARB_pixel_format && GLAD_WGL_ARB_multisample && GLAD_WGL_ARB_create_context)
	{
		// enable MSAA
		const int attributes[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
			WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,     (int)24,
			WGL_RED_BITS_ARB,		(int)8,
			WGL_GREEN_BITS_ARB,		(int)8,
			WGL_BLUE_BITS_ARB,		(int)8,
			WGL_ALPHA_BITS_ARB,		(int)8,
			WGL_DEPTH_BITS_ARB,     (int)24,
			WGL_STENCIL_BITS_ARB,   (int)8,
			WGL_SAMPLE_BUFFERS_ARB, 1,
			WGL_SAMPLES_ARB,        4,  // 4x MSAA
			0
		};

		bool fail = wglChoosePixelFormatARB(m_hDC, attributes, nullptr, 1, &nPixelFormat, &numFormats) < 0 || numFormats == 0;
		ASUNA_ASSERT(!fail);

		result = SetPixelFormat(m_hDC, nPixelFormat, &pfd);
		ASUNA_ASSERT(result == 1);

		const int context_attributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 1,
			WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		m_RenderContext = wglCreateContextAttribsARB(m_hDC, 0, context_attributes);
		ASUNA_ASSERT(m_RenderContext);

		result = wglMakeCurrent(m_hDC, m_RenderContext);
		ASUNA_ASSERT(result == 1);

	}
	else {
		// Set pixel format.
		int nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
		ASUNA_ASSERT(nPixelFormat != 0);

		result = SetPixelFormat(m_hDC, nPixelFormat, &pfd);
		ASUNA_ASSERT(result == 1);

		// Create rendering context.
		m_RenderContext = wglCreateContext(m_hDC);
		ASUNA_ASSERT(m_RenderContext);

		// Set the rendering context as the current rendering context for this window.
		result = wglMakeCurrent(m_hDC, m_RenderContext);
		ASUNA_ASSERT(result == 1);
	}
}

void OpenGLRenderer::SetViewPort(int width, int height)
{
	glViewport(0, 0, width, height);
}


std::shared_ptr<Shader> asuna::OpenGLRenderer::CreateShader(const std::string& path, ShaderType shaderType)
{
	return OpenglShader::Create(path, shaderType);
}

std::shared_ptr<RenderItem> asuna::OpenGLRenderer::CreateRenderItem(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> vs, std::shared_ptr<Shader> ps, std::shared_ptr<ConstantBuffer> perObject)
{
	return OpenglRenderItem::Create(mesh, vs, ps, perObject);
}

std::shared_ptr<ConstantBuffer> asuna::OpenGLRenderer::CreateConstantBuffer(ConstantBufferDataType dt)
{
	return std::shared_ptr<ConstantBuffer>();
}

std::shared_ptr<Mesh> asuna::OpenGLRenderer::CreateMesh(const std::string& scenePath)
{
	auto param = AssetLoader::LoadMesh(scenePath);
	return OpenglMesh::Create(param);
}

std::shared_ptr<RenderTarget> asuna::OpenGLRenderer::CreateRenderTarget(RenderTargetDesc desc)
{
	return std::shared_ptr<RenderTarget>();
}

void asuna::OpenGLRenderer::SetRenderTarget(std::shared_ptr<RenderTarget> rt)
{
}

std::shared_ptr<RenderItemQueue> OpenGLRenderer::CreateRenderItemQueue()
{
    return std::shared_ptr<RenderItemQueue>();
}

void OpenGLRenderer::SetViewPort(int x, int y, int width, int height)
{

}


