#include "DirectX12Renderer.h"
#include <iostream>
using namespace std;

void asuna::DirectX12Renderer::Initialize(CreateRendererContextParam param)
{
	cout << "DirectX12 Initialize" << endl;
}

void asuna::DirectX12Renderer::Finalize()
{
	cout << "DirectX12 Finalize" << endl;
}

void asuna::DirectX12Renderer::ResizeResolution(int width, int height)
{
}


void asuna::DirectX12Renderer::ClearRenderTarget(float r, float g, float b, float a)
{
}

void asuna::DirectX12Renderer::CreateDeviceContext()
{
}

void asuna::DirectX12Renderer::ReleaseDeviceContext()
{
}

void asuna::DirectX12Renderer::Present()
{
}
