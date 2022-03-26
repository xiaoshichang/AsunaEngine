#include <d3dcompiler.h>
#include <iostream>

#include "DirectX11Renderer.h"
#include "../../Foundation/Platform/Assert.h"
#include "DirectX11RenderContext.h"
#include "DirectX11VertexBuffer.h"
#include "DirectX11Shader.h"
#include "DirectX11RenderItem.h"

#include "../../AssetLoader/AssetLoader.h"

using namespace asuna;
using namespace std;

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


void DirectX11Renderer::Initialize(CreateRendererContextParam param)
{
	m_APIType = RenderAPIType::Directx11;
	m_ResolutionWidth = param.m_ResolutionWith;
	m_ResolutionHeight = param.m_ResolutionHeight;
	m_Surface.Type = param.m_SurfaceType;
	m_Surface.HWND = param.m_HWND;
	CreateDeviceContext();
	InitTriangle();
}

void DirectX11Renderer::Finalize()
{
	ReleaseDeviceContext();
}


void DirectX11Renderer::ResizeResolution(int width, int height)
{
	auto context = dynamic_pointer_cast<DirectX11RenderContext>(m_Context);
	context->m_DeviceContext->OMSetRenderTargets(0, 0, 0);

	m_RenderTargetView->Release();
	HRESULT hr;
	hr = m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	ASUNA_ASSERT(SUCCEEDED(hr));
	ID3D11Texture2D* pBuffer;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
	if (pBuffer == nullptr)
	{
		ASUNA_ASSERT(false);
		return;
	}
	hr = context->m_Device->CreateRenderTargetView(pBuffer, NULL,&m_RenderTargetView);
	ASUNA_ASSERT(SUCCEEDED(hr));
	pBuffer->Release();

	context->m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, NULL);
	SetViewPort(width, height);
}


void DirectX11Renderer::SetViewPort(int width, int height)
{
	D3D11_VIEWPORT viewport;
	// Setup the viewport for rendering.
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	// Create the viewport.
	auto context = dynamic_pointer_cast<DirectX11RenderContext>(m_Context);
	context->m_DeviceContext->RSSetViewports(1, &viewport);
}


void DirectX11Renderer::ClearRenderTarget(float r, float g, float b, float a)
{
	auto context = dynamic_pointer_cast<DirectX11RenderContext>(m_Context);
	float color[4] = { r, g, b, a };
	// Clear the back buffer.
	context->m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, color);
	// Clear the depth buffer.
	context->m_DeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void asuna::DirectX11Renderer::Present()
{
	// Lock to screen refresh rate.
	// m_swapChain->Present(1, 0);
	// Present as fast as possible.
	m_swapChain->Present(0, 0);
}


void DirectX11Renderer::CreateDeviceContext()
{
	auto context = make_shared<DirectX11RenderContext>();
	m_Context = context;

	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes;
	size_t stringLength;
	DXGI_MODE_DESC* displayModeList;
	
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	ASUNA_ASSERT(result >= 0);

	// Enumerate the primary adapter output (monitor).
	result = factory->EnumAdapters(0, &adapter);
	ASUNA_ASSERT(result >= 0);

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	ASUNA_ASSERT(result >= 0);

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	ASUNA_ASSERT(result >= 0);

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	ASUNA_ASSERT(displayModeList);

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	ASUNA_ASSERT(result >= 0);

	{
		DXGI_ADAPTER_DESC adapterDesc;
		result = adapter->GetDesc(&adapterDesc);
		ASUNA_ASSERT(result >= 0);
		auto videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
		// Convert the name of the video card to a character array and store it.
		char videoCardDescription[128];
		error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
		ASUNA_ASSERT(error == 0);
	}

	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = m_ResolutionWidth;
	swapChainDesc.BufferDesc.Height = m_ResolutionHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	if (m_Surface.Type == RenderSurfaceType::WindowsApplication)
	{
		swapChainDesc.OutputWindow = m_Surface.HWND;
	}
	else
	{
		ASUNA_ASSERT(false);
	}

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (false)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &context->m_Device, NULL, &context->m_DeviceContext);
	ASUNA_ASSERT(result >= 0);


	// create render target from swapchain back buffer.
	{
		// Get the pointer to the back buffer.
		ID3D11Texture2D* backBufferPtr;
		result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		ASUNA_ASSERT(result >= 0);

		// Create the render target view with the back buffer pointer.
		result = context->m_Device->CreateRenderTargetView(backBufferPtr, NULL, &m_RenderTargetView);
		ASUNA_ASSERT(result >= 0);

		// Release pointer to the back buffer as we no longer need it.
		backBufferPtr->Release();
		backBufferPtr = 0;
	}
	

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	// Set up the description of the depth buffer.
	depthBufferDesc.Width = m_ResolutionWidth;
	depthBufferDesc.Height = m_ResolutionHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	result = context->m_Device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	ASUNA_ASSERT(result >= 0);

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = context->m_Device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	ASUNA_ASSERT(result >= 0);
	// Set the depth stencil state.
	context->m_DeviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = context->m_Device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	ASUNA_ASSERT(result >= 0);

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	context->m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = context->m_Device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	ASUNA_ASSERT(result >= 0);

	// Now set the rasterizer state.
	context->m_DeviceContext->RSSetState(m_rasterState);

	SetViewPort(m_ResolutionWidth, m_ResolutionHeight);
}

void DirectX11Renderer::ReleaseDeviceContext()
{
	if (m_swapChain != nullptr)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}

	if (m_RenderTargetView != nullptr)
	{
		m_RenderTargetView->Release();
		m_RenderTargetView = nullptr;
	}

	if (m_depthStencilBuffer != nullptr)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
	}

	if (m_depthStencilState != nullptr)
	{
		m_depthStencilState->Release();
		m_depthStencilState = nullptr;
	}

	if (m_depthStencilView != nullptr)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}

	if (m_rasterState != nullptr)
	{
		m_rasterState->Release();
		m_rasterState = nullptr;
	}

}



void DirectX11Renderer::InitTriangle()
{

	auto param = AssetLoader::LoadMesh("Assets\\Models\\keqin.fbx");
	auto mesh = DirectX11Mesh::Create(param);
	auto vertexShader = DirectX11VextexShader::Create("Assets\\Shaders\\triangle.vs");
	auto pixelShader = DirectX11PixelShader::Create("Assets\\Shaders\\triangle.ps");
	auto constantBuffer = DirectX11ConstantBuffer::Create();
	auto renderItem = make_shared<DirectX11RenderItem>(mesh, vertexShader, pixelShader, constantBuffer);
	AddRenderItem(renderItem);
}
