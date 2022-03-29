#include "EmptyRenderer.h"

using namespace asuna;

void asuna::EmptyRenderer::Initialize(CreateRendererContextParam param)
{
}

void asuna::EmptyRenderer::Finalize()
{
}

void asuna::EmptyRenderer::ResizeResolution(int width, int height)
{
}


void asuna::EmptyRenderer::CreateDeviceContext()
{
}

void asuna::EmptyRenderer::ReleaseDeviceContext()
{
}

void asuna::EmptyRenderer::Present()
{
}

std::shared_ptr<Shader> asuna::EmptyRenderer::CreateShader(const std::string& scenePath, ShaderType shaderType)
{
    return std::shared_ptr<Shader>();
}

std::shared_ptr<RenderItem> asuna::EmptyRenderer::CreateRenderItem(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> pixelShader, std::shared_ptr<ConstantBuffer> constantBuffer)
{
    return std::shared_ptr<RenderItem>();
}

std::shared_ptr<ConstantBuffer> asuna::EmptyRenderer::CreateConstantBuffer()
{
    return std::shared_ptr<ConstantBuffer>();
}

void asuna::EmptyRenderer::ClearRenderTarget(std::shared_ptr<RenderTarget> rt, float r, float g, float b, float a)
{
}

std::shared_ptr<Mesh> asuna::EmptyRenderer::CreateMesh(const std::string& scenePath)
{
    return std::shared_ptr<Mesh>();
}

std::shared_ptr<RenderTarget> asuna::EmptyRenderer::CreateRenderTarget(RenderTargetDesc desc)
{
    return std::shared_ptr<RenderTarget>();
}

void asuna::EmptyRenderer::SetRenderTarget(std::shared_ptr<RenderTarget> rt)
{
}
