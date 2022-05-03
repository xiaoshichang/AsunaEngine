//
// Created by xiao on 2022/4/13.
//

#include "OpenglRenderTarget.h"
#include "../../Foundation/Platform/Assert.h"

using namespace asuna;

OpenglRenderTarget::OpenglRenderTarget(RenderTargetDesc desc) :
    RenderTarget(desc)
{
    GenFrameBuffer();
    GenTexture();
    GenDepthBuffer();
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
    GenTexture();
    GenDepthBuffer();
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
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);
}

void OpenglRenderTarget::BindTextureToFrameBuffer() const
{
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_Texture, 0);
    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
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
}


