//
// Created by xiao on 2022/4/18.
//

#include "OpenglTexture.h"

using namespace asuna;
using namespace std;

OpenglTexture::OpenglTexture(const shared_ptr<RawTexture> &rawImage, GLuint texture) :
    ImageTexture(rawImage),
    m_Texture(texture)
{
}

OpenglTexture::~OpenglTexture()
{
    if (m_Texture != 0)
    {
        glDeleteTextures(1, &m_Texture);
        m_Texture = 0;
    }
}

shared_ptr<Texture> OpenglTexture::Create(const shared_ptr<RawTexture> &rawTexture)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rawTexture->GetWidth(), rawTexture->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, rawTexture->GetData());
    glGenerateMipmap(GL_TEXTURE_2D);
    return make_shared<OpenglTexture>(rawTexture, texture);
}


