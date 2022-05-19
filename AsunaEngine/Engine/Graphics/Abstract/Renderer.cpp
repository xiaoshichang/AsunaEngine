
#include <algorithm>
#include "Renderer.h"

using namespace asuna;
using namespace std;

Renderer* Renderer::Instance = nullptr;

Renderer::Renderer() : m_Context(nullptr)
{
    m_APIType = RenderAPIType::None;
}

Renderer::~Renderer() = default;

shared_ptr<RenderContext> asuna::Renderer::GetContext()
{
	return m_Context;
}

void Renderer::CreateDefaultRasterizationState()
{
	RasterizationStateDesc desc{};
	desc.CullMode = RasterizationCullMode::Back;
	desc.FillMode = RasterizationFillMode::Solid;
	desc.FrontCounterClockWise = false;
	m_DefaultRasterizationState = CreateRasterizationState(desc);
}

RenderAPIType Renderer::GetRenderAPIType() const
{
    return m_APIType;
}

