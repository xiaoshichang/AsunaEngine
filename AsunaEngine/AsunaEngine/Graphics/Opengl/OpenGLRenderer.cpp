#include <cstdint>
#include <fstream>
#include "OpenGLRenderer.h"
#include "OpenglMesh.h"
#include "OpenglRenderItem.h"
#include "OpenglMaterial.h"
#include "OpenglRenderItemQueue.h"
#include "OpenglConstantBuffer.h"
#include "OpenglDepthStencilState.h"
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

void OpenGLRenderer::Initialize(CreateRendererContextParam param)
{
	m_APIType = RenderAPIType::Opengl;
	m_ResolutionWidth = param.m_ResolutionWith;
	m_ResolutionHeight = param.m_ResolutionHeight;
	m_HWND = param.m_HWND;

	CreateDeviceContext();
}

void OpenGLRenderer::Finalize()
{
}

void OpenGLRenderer::ResizeResolution(int width, int height)
{
	glViewport(0, 0, width, height);
}

void OpenGLRenderer::ClearRenderTarget(shared_ptr<RenderTarget> rt, float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::Present()
{
	// Present the back buffer to the screen since rendering is complete.
	SwapBuffers(m_hDC);
}

void OpenGLRenderer::CreateDeviceContext()
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
	glFrontFace(GL_CCW);
	// Enable back face culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	SetViewPort(0, 0, m_ResolutionWidth, m_ResolutionHeight);
	wglSwapIntervalEXT(1); // disable vertical sync to get high fps.
}

void OpenGLRenderer::ReleaseDeviceContext()
{
	if (m_RenderContext != 0)
	{
		wglDeleteContext(m_RenderContext);
		m_RenderContext = 0;
	}
	m_hDC = 0;
}

void OpenGLRenderer::LoadWGL()
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

	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(m_RenderContext);
	ReleaseDC(TemphWnd, TemphDC);
	DestroyWindow(TemphWnd);
}

void OpenGLRenderer::CreateGLContext()
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


shared_ptr<Shader> OpenGLRenderer::CreateShader(const string& path, ShaderType shaderType)
{
	return OpenglShader::Create(path, shaderType);
}

shared_ptr<RenderItem>
OpenGLRenderer::CreateRenderItem(
        const shared_ptr<Mesh>& mesh,
        const vector<shared_ptr<Material>>& materials,
        const shared_ptr<ConstantBuffer>& perObject)
{
	return OpenglRenderItem::Create(mesh, materials, perObject);
}

shared_ptr<RenderItem>
OpenGLRenderer::CreateRenderItem(
        const shared_ptr<Mesh> &mesh,
        const shared_ptr<ConstantBuffer> &perObject)
{
    return OpenglRenderItem::Create(mesh, perObject);
}

shared_ptr<ConstantBuffer> OpenGLRenderer::CreateConstantBuffer(ConstantBufferDataType dt, int size)
{
	return OpenglConstantBuffer::Create(dt, size);
}

shared_ptr<Mesh> OpenGLRenderer::CreateMesh(const string& scenePath)
{
	auto param = AssetLoader::LoadMesh(scenePath);
	return OpenglMesh::Create(param);
}

shared_ptr<RenderTarget> OpenGLRenderer::CreateRenderTarget(RenderTargetDesc desc)
{
	return shared_ptr<RenderTarget>();
}

void OpenGLRenderer::SetRenderTarget(shared_ptr<RenderTarget> rt)
{
}

shared_ptr<RenderItemQueue> OpenGLRenderer::CreateRenderItemQueue()
{
    return make_shared<OpenglRenderItemQueue>();
}

void OpenGLRenderer::SetViewPort(int x, int y, int width, int height)
{
    auto w = width == -1 ? m_ResolutionWidth : width;
    auto h = height == -1 ? m_ResolutionHeight : height;
    glViewport(x, y, w, h);
}

shared_ptr<DepthStencilState> OpenGLRenderer::CreateDepthStencilState()
{
    return make_shared<OpenglDepthStencilState>();
}

std::shared_ptr<Material> OpenGLRenderer::CreateMaterial(const string &materialPath)
{
    return OpenglMaterial::Create(materialPath);
}

void OpenGLRenderer::MakeCurrentContext()
{
    auto result = wglMakeCurrent(m_hDC, m_RenderContext);
    ASUNA_ASSERT(result == 1);
}


