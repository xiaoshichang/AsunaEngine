#include <d3dcompiler.h>
#include <iostream>
#include "../../Foundation/Platform/Assert.h"
#include "../../AssetLoader/AssetLoader.h"

#include "DirectX11Renderer.h"
#include "DirectX11RenderContext.h"
#include "DirectX11VertexBuffer.h"
#include "DirectX11Shader.h"
#include "DirectX11RenderItem.h"
#include "DirectX11DepthStencilState.h"
#include "DirectX11Texture.h"
#include "DirectX11Material.h"


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
    m_ResolutionWidth = width;
    m_ResolutionHeight = height;

	auto context = dynamic_pointer_cast<DirectX11RenderContext>(m_Context);
	auto mainRT = context->m_MainRT;

    SetRenderTarget(nullptr);
    ClearRenderTarget(nullptr, 0.1f, 0.2f, 0.3f, 1.0f);

	if (mainRT != nullptr)
	{
        mainRT->Resize(width, height);
	}
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

shared_ptr<Shader> asuna::DirectX11Renderer::CreateShader(const string& path, ShaderType shaderType)
{
	if (shaderType == ShaderType::VertexShader)
	{
		return DirectX11VertexShader::Create(path);
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

shared_ptr<ConstantBuffer> DirectX11Renderer::CreateConstantBuffer(ConstantBufferDataType dt, int size)
{
	auto cb = DirectX11ConstantBuffer::Create(dt, size);
	return cb;
}

shared_ptr<RenderItem> asuna::DirectX11Renderer::CreateRenderItem(
        const std::shared_ptr<Mesh>& mesh,
        const vector<std::shared_ptr<Material>>& materials,
        const std::shared_ptr<ConstantBuffer>& perObject)
{
	return DirectX11RenderItem::Create(mesh, materials, perObject);
}

shared_ptr<RenderItem>
DirectX11Renderer::CreateRenderItem(const shared_ptr<Mesh> &mesh, const shared_ptr<ConstantBuffer> &perObject)
{
    return DirectX11RenderItem::Create(mesh, perObject);
}


void DirectX11Renderer::ClearRenderTarget(shared_ptr<RenderTarget> rt, float r, float g, float b, float a)
{
	auto context = dynamic_pointer_cast<DirectX11RenderContext>(m_Context);
	float color[4] = { r, g, b, a };

	if (rt == nullptr)
	{
        auto rtv = context->m_MainRT->GetRenderTargetView();
        auto dsv = context->m_MainRT->GetDepthStencilView();
        if (rtv != nullptr)
        {
            context->m_DeviceContext->ClearRenderTargetView(rtv, color);
        }
        if (dsv != nullptr)
        {
            context->m_DeviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0, 0);
        }
	}
	else
	{
		auto dx11rt = dynamic_pointer_cast<DirectX11RenderTarget>(rt);
		auto rtv = dx11rt->GetRenderTargetView();
        auto dsv = dx11rt->GetDepthStencilView();
        if (rtv != nullptr)
        {
            context->m_DeviceContext->ClearRenderTargetView(rtv, color);
        }
        if (dsv != nullptr)
        {
            context->m_DeviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0, 0);
        }
	}
}

void asuna::DirectX11Renderer::Present()
{
	// Lock to screen refresh rate.
	// m_swapChain->Present(1, 0);
	// Present as fast as possible.
	auto context = dynamic_pointer_cast<DirectX11RenderContext>(m_Context);
	context->m_swapChain->Present(1, 0);
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

    RenderTargetDesc desc{};
    desc.usage = RenderTargetUsage::Default;
    desc.width = m_ResolutionWidth;
    desc.height = m_ResolutionHeight;
	auto renderTarget = DirectX11RenderTarget::CreateFromSwapChain(desc, device, swapChain);
	SetRasterizerState(device, deviceContext);
	m_Context = make_shared<DirectX11RenderContext>(device, deviceContext, swapChain, renderTarget);
}

void DirectX11Renderer::ReleaseDeviceContext()
{
	if (m_Context != nullptr)
	{
		m_Context.reset();
		m_Context = nullptr;
	}

	if (m_rasterState != nullptr)
	{
		m_rasterState->Release();
		m_rasterState = nullptr;
	}

}

shared_ptr<Mesh> asuna::DirectX11Renderer::CreateMesh(const string& scenePath)
{
	auto param = AssetLoader::LoadMesh(scenePath);
	auto mesh = DirectX11Mesh::Create(param);
	return mesh;
}

shared_ptr<Mesh> DirectX11Renderer::CreateMesh(const std::shared_ptr<MeshCreateParam>& param)
{
    return DirectX11Mesh::Create(param);
}

shared_ptr<RenderTarget> asuna::DirectX11Renderer::CreateRenderTarget(RenderTargetDesc desc)
{
    auto context = dynamic_pointer_cast<DirectX11RenderContext>(m_Context);
	return DirectX11RenderTarget::Create(desc, context->m_Device);
}

void asuna::DirectX11Renderer::SetRenderTarget(shared_ptr<RenderTarget> rt)
{
	auto context = dynamic_pointer_cast<DirectX11RenderContext>(m_Context);
	
	if (rt == nullptr)
	{
		auto mainRT = context->m_MainRT;
		auto rtv = mainRT->GetRenderTargetView();
        auto dsv = mainRT->GetDepthStencilView();
		context->m_DeviceContext->OMSetRenderTargets(1, &rtv, dsv);
	}
	else
	{
		auto dx11rt = dynamic_pointer_cast<DirectX11RenderTarget>(rt);
		auto rtv = dx11rt->GetRenderTargetView();
        auto dsv = dx11rt->GetDepthStencilView();
		context->m_DeviceContext->OMSetRenderTargets(1, &rtv, dsv);
	}
}

void DirectX11Renderer::SetViewPort(int x, int y, int width, int height)
{
    auto context = dynamic_pointer_cast<DirectX11RenderContext>(m_Context);
    D3D11_VIEWPORT viewport;
    viewport.Width = width == -1 ? (float)m_ResolutionWidth : (float)width;
    viewport.Height = height == -1 ? (float)m_ResolutionHeight : (float)height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = (float)x;
    viewport.TopLeftY = (float)y;
    // Create the viewport.
    context->m_DeviceContext->RSSetViewports(1, &viewport);
}

shared_ptr<Material> DirectX11Renderer::CreateMaterial(const string &materialPath, MaterialType mt)
{
    return std::make_shared<DirectX11Material>(materialPath, mt);
}

shared_ptr<DepthStencilState> DirectX11Renderer::CreateDepthStencilState()
{
    return make_shared<DirectX11DepthStencilState>();
}

shared_ptr<Texture> DirectX11Renderer::CreateTexture(const string &path)
{
    auto raw = AssetLoader::LoadRawTexture(path);
    return DirectX11Texture::Create(raw);
}






