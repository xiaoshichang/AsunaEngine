#pragma once
#include "Texture.h"

namespace asuna
{
    enum class RenderTargetUsage
    {
        Default,
        ShadowMap
    };

    struct RenderTargetDesc
    {
        RenderTargetUsage usage;
        int width;
        int height;
    };


    class RenderTarget : public RTTexture
	{
	public:
        RenderTarget() = delete;
		explicit RenderTarget(RenderTargetDesc desc) :
            RTTexture(),
            m_Desc(desc)
		{
		}

		~RenderTarget() override = default;
        virtual void Resize(int width, int height);
        int GetWidth() {return m_Desc.width;}
        int GetHeight() {return m_Desc.height;}

    protected:
        RenderTargetDesc m_Desc;

	};

}