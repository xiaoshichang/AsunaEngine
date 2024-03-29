#include "DirectX11Shader.h"
#include <d3dcompiler.h>
#include <iostream>

#include "../../Foundation/Platform/Assert.h"
#include "DirectX11Renderer.h"
#include "DirectX11RenderContext.h"

using namespace asuna;
using namespace std;

shared_ptr<DirectX11VertexShader> DirectX11VertexShader::Create(const std::string& path)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3DBlob* byteCode;
	ID3D11VertexShader* vs;
	ID3D11InputLayout* layout;
	
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	std::wstring _path = std::wstring(path.begin(), path.end());
	result = D3DCompileFromFile(_path.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", dwShaderFlags, 0, &byteCode, &errorMessage);
	if (FAILED(result)) 
	{
		OutputDebugStringA(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()));
		std::cout << reinterpret_cast<const char*>(errorMessage->GetBufferPointer()) << std::endl;
	}
	auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Instance->GetContext());
	context->m_Device->CreateVertexShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &vs);

	ASUNA_ASSERT(SUCCEEDED(result));
	
	// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_input_element_desc
	// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_input_classification
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	context->m_Device->CreateInputLayout(ied, 3, byteCode->GetBufferPointer(), byteCode->GetBufferSize(), &layout);
    if (layout == nullptr)
    {
        ASUNA_ASSERT(false);
    }
	return make_shared<DirectX11VertexShader>(vs, layout, byteCode);
}

void DirectX11VertexShader::Bind()
{
    auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Instance->GetContext());
    context->m_DeviceContext->IASetInputLayout(m_Layout);
    context->m_DeviceContext->VSSetShader(m_VS, 0, 0);
}

shared_ptr<DirectX11PixelShader> DirectX11PixelShader::Create(const std::string& path)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3DBlob* byteCode = nullptr;
	ID3D11PixelShader* ps;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	std::wstring _path = std::wstring(path.begin(), path.end());
	result = D3DCompileFromFile(_path.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", dwShaderFlags, 0, &byteCode, &errorMessage);
	if (FAILED(result))
	{
		OutputDebugStringA(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()));
		std::cout << reinterpret_cast<const char*>(errorMessage->GetBufferPointer()) << std::endl;
	}
	auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Instance->GetContext());
	context->m_Device->CreatePixelShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &ps);
	ASUNA_ASSERT(SUCCEEDED(result));
	return make_shared<DirectX11PixelShader>(ps, byteCode);
}

void DirectX11PixelShader::Bind()
{
    auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Instance->GetContext());
    context->m_DeviceContext->PSSetShader(m_PS, 0, 0);
}
