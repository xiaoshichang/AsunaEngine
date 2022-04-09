
#include <algorithm>
#include "Renderer.h"

using namespace asuna;
using namespace std;

Renderer* Renderer::Current = nullptr;


shared_ptr<RenderContext> asuna::Renderer::GetContext()
{
	return m_Context;
}


bool Renderer::CheckLeftHandRenderAPI() const
{
    switch (m_APIType)
    {
        case RenderAPIType::Directx11:
        case RenderAPIType::Directx12:
            return true;
        case RenderAPIType::Opengl:
        case RenderAPIType::Opengles:
            return false;
        default:
            ASUNA_ASSERT(false);
            return false;
    }
}

std::shared_ptr<Material> Renderer::CreateMaterial(const shared_ptr<Shader> &vs, const shared_ptr<Shader> &ps)
{
    return Material::Create(vs, ps);
}
