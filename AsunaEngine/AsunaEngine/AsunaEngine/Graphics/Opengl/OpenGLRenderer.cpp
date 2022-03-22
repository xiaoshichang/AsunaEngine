#include "OpenGLRenderer.h"
#include "../../Foundation/Platform/Assert.h"
#include <cstdint>
#include <fstream>
#include "../../Foundation/Math/AMath.h"
using namespace std;

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

float g_positionX = 0, g_positionY = 0, g_positionZ = -10;
float g_rotationX = 0, g_rotationY = 0, g_rotationZ = 0;
asuna::Matrix4x4f g_worldMatrix;
asuna::Matrix4x4f g_viewMatrix;
asuna::Matrix4x4f g_projectionMatrix;

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

typedef struct VectorType
{
	union {
		struct { float x, y, z; };
		struct { float r, g, b; };
	};
} VectorType;
typedef struct VertexType
{
	VectorType position;
	VectorType color;
} VertexType;


void asuna::OpenGLRenderer::Initialize()
{
	CreateDeviceContext();
	InitializeBuffers();
	InitializeShader();

}

void asuna::OpenGLRenderer::Finalize()
{
}

void asuna::OpenGLRenderer::Render()
{
	ClearRenderTarget(0.2f, 0.2f, 0.3f, 1.0f);
	CalculateCameraPosition();
	// Set the color shader as the current shader program and set the matrices that it will use for rendering.
	glUseProgram(g_shaderProgram);
	SetShaderParameters(g_worldMatrix, g_viewMatrix, g_projectionMatrix);
	// Render the model using the color shader.
	RenderBuffers();
	// Present the back buffer to the screen since rendering is complete.
	SwapBuffers(m_hDC);
}

void asuna::OpenGLRenderer::ClearRenderTarget(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	glFrontFace(GL_CW);

	// Enable back face culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, 1024, 768);
	wglSwapIntervalEXT(0); // disable vertical sync to get high fps.
}

void asuna::OpenGLRenderer::ReleaseDeviceContext()
{
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
	WndClassEx.lpszClassName = L"InitWindow";
	RegisterClassEx(&WndClassEx);
	HWND TemphWnd = CreateWindowEx(WS_EX_APPWINDOW, WndClassEx.lpszClassName, L"InitWindow", Style, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
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

	auto windowsSurface = (RenderSurfaceWindowsApplication*)m_Surface;
	m_hDC = GetDC(windowsSurface->HWND);

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

bool asuna::OpenGLRenderer::InitializeBuffers()
{
	VertexType vertices[] = {
			{{  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }},
			{{  1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }},
			{{ -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }},
			{{ -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 0.0f }},
			{{  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 1.0f }},
			{{  1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f, 1.0f }},
			{{ -1.0f, -1.0f, -1.0f }, { 0.5f, 1.0f, 0.5f }},
			{{ -1.0f, -1.0f,  1.0f }, { 1.0f, 0.5f, 1.0f }},
	};
	uint16_t indices[] = { 1, 2, 3, 3, 2, 6, 6, 7, 3, 3, 0, 1, 0, 3, 7, 7, 6, 4, 4, 6, 5, 0, 7, 4, 1, 0, 4, 1, 4, 5, 2, 1, 5, 2, 5, 6 };

	// Set the number of vertices in the vertex array.
	g_vertexCount = sizeof(vertices) / sizeof(VertexType);

	// Set the number of indices in the index array.
	g_indexCount = sizeof(indices) / sizeof(uint16_t);

	// Allocate an OpenGL vertex array object.
	glGenVertexArrays(1, &g_vertexArrayId);

	// Bind the vertex array object to store all the buffers and vertex attributes we create here.
	glBindVertexArray(g_vertexArrayId);

	// Generate an ID for the vertex buffer.
	glGenBuffers(1, &g_vertexBufferId);

	// Bind the vertex buffer and load the vertex (position and color) data into the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, g_vertexCount * sizeof(VertexType), vertices, GL_STATIC_DRAW);

	// Enable the two vertex array attributes.
	glEnableVertexAttribArray(0);  // Vertex position.
	glEnableVertexAttribArray(1);  // Vertex color.

	// Specify the location and format of the position portion of the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexType), 0);

	// Specify the location and format of the color portion of the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferId);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexType), (char*)NULL + (3 * sizeof(float)));

	// Generate an ID for the index buffer.
	glGenBuffers(1, &g_indexBufferId);

	// Bind the index buffer and load the index data into it.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_indexCount * sizeof(uint16_t), indices, GL_STATIC_DRAW);

	return true;
}

bool asuna::OpenGLRenderer::InitializeShader()
{
	const char* vertexShaderBuffer;
	const char* fragmentShaderBuffer;
	int status;

	// Load the vertex shader source file into a text buffer.
	vertexShaderBuffer = LoadShaderSourceFile("Assets\\Shaders\\color.vs");
	if (!vertexShaderBuffer)
	{
		return false;
	}

	// Load the fragment shader source file into a text buffer.
	fragmentShaderBuffer = LoadShaderSourceFile("Assets\\Shaders\\color.ps");
	if (!fragmentShaderBuffer)
	{
		return false;
	}

	// Create a vertex and fragment shader object.
	g_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	g_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Copy the shader source code strings into the vertex and fragment shader objects.
	glShaderSource(g_vertexShader, 1, &vertexShaderBuffer, NULL);
	glShaderSource(g_fragmentShader, 1, &fragmentShaderBuffer, NULL);

	// Release the vertex and fragment shader buffers.
	delete[] vertexShaderBuffer;
	vertexShaderBuffer = 0;

	delete[] fragmentShaderBuffer;
	fragmentShaderBuffer = 0;

	// Compile the shaders.
	glCompileShader(g_vertexShader);
	glCompileShader(g_fragmentShader);

	// Check to see if the vertex shader compiled successfully.
	glGetShaderiv(g_vertexShader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		// If it did not compile then write the syntax error message out to a text file for review.
		return false;
	}

	// Check to see if the fragment shader compiled successfully.
	glGetShaderiv(g_fragmentShader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		// If it did not compile then write the syntax error message out to a text file for review.
		return false;
	}

	// Create a shader program object.
	g_shaderProgram = glCreateProgram();

	// Attach the vertex and fragment shader to the program object.
	glAttachShader(g_shaderProgram, g_vertexShader);
	glAttachShader(g_shaderProgram, g_fragmentShader);

	// Bind the shader input variables.
	glBindAttribLocation(g_shaderProgram, 0, "inputPosition");
	glBindAttribLocation(g_shaderProgram, 1, "inputColor");

	// Link the shader program.
	glLinkProgram(g_shaderProgram);

	// Check the status of the link.
	glGetProgramiv(g_shaderProgram, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		// If it did not link then write the syntax error message out to a text file for review.
		return false;
	}

	return true;
}

bool asuna::OpenGLRenderer::SetShaderParameters(float* worldMatrix, float* viewMatrix, float* projectionMatrix)
{
	unsigned int location;

	// Set the world matrix in the vertex shader.
	location = glGetUniformLocation(g_shaderProgram, "worldMatrix");
	if (location == -1)
	{
		return false;
	}
	glUniformMatrix4fv(location, 1, false, worldMatrix);

	// Set the view matrix in the vertex shader.
	location = glGetUniformLocation(g_shaderProgram, "viewMatrix");
	if (location == -1)
	{
		return false;
	}
	glUniformMatrix4fv(location, 1, false, viewMatrix);

	// Set the projection matrix in the vertex shader.
	location = glGetUniformLocation(g_shaderProgram, "projectionMatrix");
	if (location == -1)
	{
		return false;
	}
	glUniformMatrix4fv(location, 1, false, projectionMatrix);

	return true;
}

char* asuna::OpenGLRenderer::LoadShaderSourceFile(const char* filename)
{
	ifstream fin;
	int fileSize;
	char input;
	char* buffer;


	// Open the shader source file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return 0;
	}

	// Initialize the size of the file.
	fileSize = 0;

	// Read the first element of the file.
	fin.get(input);

	// Count the number of elements in the text file.
	while (!fin.eof())
	{
		fileSize++;
		fin.get(input);
	}

	// Close the file for now.
	fin.close();

	// Initialize the buffer to read the shader source file into.
	buffer = new char[fileSize + 1];
	if (!buffer)
	{
		return 0;
	}

	// Open the shader source file again.
	fin.open(filename);

	// Read the shader text file into the buffer as a block.
	fin.read(buffer, fileSize);

	// Close the file.
	fin.close();

	// Null terminate the buffer.
	buffer[fileSize] = '\0';

	return buffer;
}

void asuna::OpenGLRenderer::CalculateCameraPosition()
{
	Vector3f up, position, lookAt;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = 10;
	position.y = 10;
	position.z = 10;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 0.0f;
	
	// Finally create the view matrix from the three updated vectors.
	g_viewMatrix = BuildMatrixViewLookatRH(position, lookAt, up).TransposeCopy();
	// Set the field of view and screen aspect ratio.
	float fieldOfView, screenAspect;
	fieldOfView = PI / 4.0f;
	screenAspect = (float)1024 / (float)768;

	// Build the perspective projection matrix.
	g_projectionMatrix = BuildMatrixPerspectiveFovRH(fieldOfView, screenAspect, 0.1f, 1000.0f).TransposeCopy();
	g_worldMatrix = g_worldMatrix.Identity();
}

void asuna::OpenGLRenderer::RenderBuffers()
{
	// Bind the vertex array object that stored all the information about the vertex and index buffers.
	glBindVertexArray(g_vertexArrayId);

	// Render the vertex buffer using the index buffer.
	glDrawElements(GL_TRIANGLES, g_indexCount, GL_UNSIGNED_SHORT, 0);
}
