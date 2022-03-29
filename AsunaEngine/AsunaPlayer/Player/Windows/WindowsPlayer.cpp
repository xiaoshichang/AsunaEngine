#include "WindowsPlayer.h"
#include "AsunaEngine/Graphics/Renderer.h"
#include "AsunaEngine/AssetLoader/AssetLoader.h"

void asuna::WindowsPlayer::Initialize(ApplicationInitParam param)
{
	WindowsApplication::Initialize(param);
	auto mesh = Renderer::Current->CreateMesh("Assets\\Models\\keqin.fbx");
	auto vertexShader = Renderer::Current->CreateShader("Assets\\Shaders\\triangle.vs", ShaderType::VertexShader);
	auto pixelShader = Renderer::Current->CreateShader("Assets\\Shaders\\triangle.ps", ShaderType::PixelShader);
	auto constantBuffer = Renderer::Current->CreateConstantBuffer();
	auto renderItem = Renderer::Current->CreateRenderItem(mesh, vertexShader, pixelShader, constantBuffer);
	Renderer::Current->AddRenderItem(renderItem);
}

void asuna::WindowsPlayer::Render()
{
    Renderer::Current->RenderScene(nullptr);
    Renderer::Current->Present();
}
