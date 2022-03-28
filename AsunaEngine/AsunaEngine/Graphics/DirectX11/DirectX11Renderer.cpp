#include <d3dcompiler.h>
#include <iostream>
#include "../../Foundation/Platform/Assert.h"
#include "../../AssetLoader/AssetLoader.h"

#include "DirectX11Renderer.h"
#include "DirectX11RenderContext.h"
#include "DirectX11VertexBuffer.h"
#include "DirectX11Shader.h"
#include "DirectX11RenderItem.h"
#include "DirectX11RenderTarget.h"
#include "DirectX11ConstantBuffer.h"


using namespace asuna;
using namespace std;


void DirectX11Renderer::Initialize(CreateRendererContextParam param)
{
	m_APIType = RenderAPIType::Directx11;
	m_ResolutionWidth = param.m_ResolutionWith;
	m_ResolutionHeight = param.m_ResolutionHeight;
	m_HWND = param.m_HWND;
	CreateDeviceContext();
}

void DirectX11Renderer::Finalize()
{
	ReleaseDeviceContext();
}

/// https://docs.microsoft.com/en-us/windows/win32/direct3ddxgi/d3d10-graphics-programming-guide-dxgi#handling-window-resizing
void DirectX11Renderer::ResizeResolution(int width, int height)
{
	auto context = dynamic_pointer_cast<DirectX11RenderContext>(m_Context);
	auto mainRT = context->m_MainRT;
	
	context->m_DeviceContext->OMSetRenderTargets(0, 0, 0);

	if (mainRT != nullptr)
	{
		mainRT->GetRenderTargetView()->Release();
        mainRT->SetRenderTarget(nullptr);
	}
	
	auto hr = context->m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	ASUNA_ASSERT(SUCCEEDED(hr));
	ID3D11Texture2D* pBuffer;
	hr = context->m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
	ASUNA_ASSERT(SUCCEEDED(hr));
	auto rtv = mainRT->GetRenderTargetView();
	hr = context->m_Device->CreateRenderTargetView(pBuffer, NULL, &rtv);
	ASUNA_ASSERT(SUCCEEDED(hr));
    mainRT->SetRenderTarget(rtv);
	pBuffer->Release();
    context->m_DeviceContext->OMSetRenderTargets(1, &rtv, nullptr);
	SetViewPort(context->m_DeviceContext, width, height);
}


void DirectX11Renderer::SetViewPort(ID3D11DeviceContext* deviceContext, int width, int height)
{
    m_ResolutionWidth = width;
    m_ResolutionHeight = height;

	D3D11_VIEWPORT viewport;
	// Setup the viewport for rendering.
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	// Create the viewport.
	deviceContext->RSSetViewports(1, &viewport);
}

void asuna::DirectX11Renderer::SetRasterizerState(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	D3D11_RASTERIZER_DESC rasterDesc;
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
	auto result = device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	ASUNA_ASSERT(result >= 0);
	// Now set the rasterizer state.
	deviceContext->RSSetState(m_rasterState);
}

void asuna::DirectX11Renderer::CreateDepthStencilState(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

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
	auto result = device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
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
	result = device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	ASUNA_ASSERT(result >= 0);
	// Set the depth stencil state.
	deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	// Create the depth stencil view.
	result = device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	ASUNA_ASSERT(result >= 0);
}

std::shared_ptr<Shader> asuna::DirectX11Renderer::CreateShader(const std::string& path, ShaderType shaderType)
{
	if (shaderType == ShaderType::VertexShader)
	{
		return DirectX11VextexShader::Create(path);
	}
	else if (shaderType == ShaderType::PixelShader)
	{
		return DirectX11PixelShader::Create(path);
	}
	else
	{
		ASUNA_ASSERT(false);
		return nullptr;
	}
}

std::shared_ptr<ConstantBuffer> DirectX11Renderer::CreateConstantBuffer()
{
	auto cb = DirectX11ConstantBuffer::Create();
	return cb;
}

std::shared_ptr<RenderItem> asuna::DirectX11Renderer::CreateRenderItem(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> vs, std::shared_ptr<Shader> ps, std::shared_ptr<ConstantBuffer> cb)
{
	return DirectX11RenderItem::Create(mesh, vs, ps, cb);
}


void DirectX11Renderer::ClearRenderTarget(std::shared_ptr<RenderTarget> rt, float r, float g, float b, float a)
{
	auto context = dynamic_pointer_cast<DirectX11RenderContext>(m_Context);
	float color[4] = { r, g, b, a };

	if (rt == nullptr)
	{
		context->m_DeviceContext->ClearRenderTargetView(context->m_MainRT->GetRenderTargetView(), color);
		context->m_DeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	else
	{
		auto dx11rt = dynamic_pointer_cast<DirectX11RenderTarget>(rt);
		auto rtv = dx11rt->GetRenderTargetView();
		context->m_DeviceContext->ClearRenderTargetView(rtv, color);
	}
}

void asuna::DirectX11Renderer::Present()
{
	// Lock to screen refresh rate.
	// m_swapChain->Present(1, 0);
	// Present as fast as possible.
	auto context = dynamic_pointer_cast<DirectX11RenderContext>(m_Context);
	context->m_swapChain->Present(0, 0);
}


void DirectX11Renderer::CreateDeviceContext()
{
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
	
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;

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
	swapChainDesc.OutputWindow = m_HWND;
	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	// Set to full screen or windowed mode.
	swapChainDesc.Windowed = true;
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
	result = D3D11CreateDeviceAndSwapChain(
		NULL, 
		D3D_DRIVER_TYPE_HARDWARE, 
		NULL, 
		0, 
		&featureLevel, 
		1,
		D3D11_SDK_VERSION, 
		&swapChainDesc, 
		&swapChain, 
		&device, 
		NULL, 
		&deviceContext
	);
	ASUNA_ASSERT(result >= 0);

	auto renderTarget = DirectX11RenderTarget::CreateFromSwapChain(device, swapChain);
	CreateDepthStencilState(device, deviceContext);
	SetRasterizerState(device, deviceContext);
	SetViewPort(deviceContext, m_ResolutionWidth, m_ResolutionHeight);

	m_Context = make_shared<DirectX11RenderContext>(device, deviceContext, swapChain, renderTarget);
}

void DirectX11Renderer::ReleaseDeviceContext()
{
	if (m_Context != nullptr)
	{
		m_Context.reset();
		m_Context = nullptr;
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

std::shared_ptr<Mesh> asuna::DirectX11Renderer::CreateMesh(const std::string& scenePath)
{
	auto param = AssetLoader::LoadMesh(scenePath);
	auto mesh = DirectX11Mesh::Create(param);
	return mesh;
}

std::shared_ptr<RenderTarget> asuna::DirectX11Renderer::CreateRenderTarget()
{
	return std::shared_ptr<RenderTarget>();
}

void asuna::DirectX11Renderer::SetRenderTarget(std::shared_ptr<RenderTarget> rt)
{
	auto context = dynamic_pointer_cast<DirectX11RenderContext>(m_Context);
	
	if (rt == nullptr)
	{
		auto mainRT = context->m_MainRT;
		auto rtv = mainRT->GetRenderTargetView();
		context->m_DeviceContext->OMSetRenderTargets(1, &rtv, m_depthStencilView);
	}
	else
	{
		auto dx11rt = dynamic_pointer_cast<DirectX11RenderTarget>(rt);
		auto rtv = dx11rt->GetRenderTargetView();
		context->m_DeviceContext->OMSetRenderTargets(1, &rtv, m_depthStencilView);
	}
}
