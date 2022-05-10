//
// Created by xiao on 2022/4/13.
//

#include "OpenglRenderTarget.h"
#include "../../Foundation/Platform/Assert.h"

using namespace asuna;

OpenglRenderTarget::OpenglRenderTarget(RenderTargetDesc desc) : RenderTarget(desc)
{
    GenFrameBuffer();
    if (desc.usage == RenderTargetUsage::Default)
    {
        GenTexture();
        GenDepthBuffer();
    }
    else if (desc.usage == RenderTargetUsage::ShadowMap)
    {
        GenDepthTexture();
    }
    BindTextureToFrameBuffer();
    Check();
}

OpenglRenderTarget::~OpenglRenderTarget()
{
    ReleaseResources();
}

void OpenglRenderTarget::Resize(int width, int height)
{
    RenderTarget::Resize(width, height);
    ReleaseResources();
    GenFrameBuffer();
    if (m_Desc.usage == RenderTargetUsage::Default)
    {
        GenTexture();
        GenDepthBuffer();
    }
    else if (m_Desc.usage == RenderTargetUsage::ShadowMap)
    {
        GenDepthTexture();
    }
    BindTextureToFrameBuffer();
    Check();
}

void OpenglRenderTarget::GenFrameBuffer()
{
    glGenFramebuffers(1, &m_FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
}

void OpenglRenderTarget::GenTexture()
{
    glGenTextures(1, &m_Texture);
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Desc.width, m_Desc.height, 0,GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void OpenglRenderTarget::GenDepthBuffer()
{
    glGenRenderbuffers(1, &m_DepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Desc.width, m_Desc.height);
}


// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
void OpenglRenderTarget::GenDepthTexture()
{
    //http://www.opengl-tutorial.org/cn/intermediate-tutorials/tutorial-16-shadow-mapping/
    glGenTextures(1, &m_DepthTexture);
    glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void OpenglRenderTarget::BindTextureToFrameBuffer() const
{
    if (m_Desc.usage == RenderTargetUsage::Default)
    {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_Texture, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);
        // Set the list of draw buffers.
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    }
    else if(m_Desc.usage == RenderTargetUsage::ShadowMap)
    {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthTexture, 0);
        glDrawBuffer(GL_NONE); // No color buffer is drawn to.
    }
}

void OpenglRenderTarget::Check() const
{
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ASUNA_ASSERT(false);
    }
}

void OpenglRenderTarget::ReleaseResources()
{
    if (m_Texture != 0)
    {
        glDeleteTextures(1, &m_Texture);
        m_Texture = 0;
    }
    if (m_DepthBuffer != 0)
    {
        glDeleteRenderbuffers(1, &m_DepthBuffer);
        m_DepthBuffer = 0;
    }
    if (m_FrameBuffer != 0)
    {
        glDeleteFramebuffers(1, &m_FrameBuffer);
        m_FrameBuffer = 0;
    }
    if (m_DepthTexture != 0)
    {
        glDeleteTextures(1, &m_DepthTexture);
        m_DepthTexture = 0;
    }
}


