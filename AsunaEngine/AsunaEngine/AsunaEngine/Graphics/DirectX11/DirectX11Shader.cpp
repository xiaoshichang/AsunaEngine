#include "DirectX11Shader.h"
#include <d3dcompiler.h>
#include <iostream>

#include "../../Foundation/Platform/Assert.h"
#include "DirectX11Renderer.h"
#include "DirectX11RenderContext.h"

using namespace asuna;

DirectX11VextexShader* DirectX11VextexShader::Create(const std::string& path)
{
	auto shader = new DirectX11VextexShader();

	HRESULT result;
	ID3D10Blob* errorMessage;
	
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	std::wstring _path = std::wstring(path.begin(), path.end());
	result = D3DCompileFromFile(_path.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", dwShaderFlags, 0, &shader->m_VSByteCode, &errorMessage);
	if (FAILED(result)) 
	{
		OutputDebugStringA(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()));
		std::cout << reinterpret_cast<const char*>(errorMessage->GetBufferPointer()) << std::endl;
	}
	auto context = (DirectX11RenderContext*)Renderer::Current->GetContext();
	context->m_Device->CreateVertexShader(shader->m_VSByteCode->GetBufferPointer(), shader->m_VSByteCode->GetBufferSize(), nullptr, &shader->m_VS);

	ASUNA_ASSERT(SUCCEEDED(result));

	// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_input_element_desc
	// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_input_classification
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	context->m_Device->CreateInputLayout(ied, 2, shader->m_VSByteCode->GetBufferPointer(), shader->m_VSByteCode->GetBufferSize(), &shader->m_Layout);
	return shader;
}

DirectX11PixelShader* DirectX11PixelShader::Create(const std::string& path)
{
	auto shader = new DirectX11PixelShader();
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3DBlob* PS;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	std::wstring _path = std::wstring(path.begin(), path.end());
	result = D3DCompileFromFile(_path.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", dwShaderFlags, 0, &PS, &errorMessage);
	if (FAILED(result))
	{
		OutputDebugStringA(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()));
		std::cout << reinterpret_cast<const char*>(errorMessage->GetBufferPointer()) << std::endl;
	}
	auto context = (DirectX11RenderContext*)Renderer::Current->GetContext();
	context->m_Device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &shader->m_PS);

	ASUNA_ASSERT(SUCCEEDED(result));
	return shader;
}
