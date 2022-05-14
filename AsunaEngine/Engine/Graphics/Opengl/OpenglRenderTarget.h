//
// Created by xiao on 2022/4/13.
//

#pragma once
#include "../Abstract/RenderTarget.h"
#include "../../3rd/Glad/include/glad/glad.h"

namespace asuna
{
    class OpenglRenderTarget : public RenderTarget
    {
    public:
        explicit OpenglRenderTarget(RenderTargetDesc desc);
        ~OpenglRenderTarget() override;
        void Resize(int width, int height) override;

        GLuint GetFrameBuffer() const {return m_FrameBuffer;}
        GLuint GetTexture() const {return  m_Texture;}
        void Bind() override;

    private:
        // http://www.opengl-tutorial.org/cn/intermediate-tutorials/tutorial-14-render-to-texture/
        void GenFrameBuffer();
        void GenTexture();
        void GenDepthBuffer();
        void GenDepthTexture();
        void BindTextureToFrameBuffer() const;
        void Check() const;
        void ReleaseResources();

    private:
        GLuint m_FrameBuffer = 0;
        GLuint m_Texture = 0;
        GLuint m_DepthBuffer = 0;
        GLuint m_DepthTexture = 0;
    };
}




