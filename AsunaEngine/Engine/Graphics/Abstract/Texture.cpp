#include "Texture.h"
#include "../../Foundation/Platform/Assert.h"

using namespace asuna;
using namespace std;

RawTexture::RawTexture(int width, int height, RawTextureFormat format, void* data):
    m_Width(width),
    m_Height(height),
    m_Format(format)
{
    int dataSize = width * height * GetPixelBytes();
    m_Data = (unsigned char*)malloc(dataSize);
    memcpy(m_Data, data, dataSize);
}

RawTexture::~RawTexture()
{
    delete[] m_Data;
    m_Data = nullptr;
}

int RawTexture::GetWidth() const
{
    return m_Width;
}

int RawTexture::GetHeight() const
{
    return m_Height;
}

int RawTexture::GetPixelBytes()
{
    if (m_Format == RawTextureFormat::R8G8B8A8)
    {
        return 4;
    }
    else if (m_Format == RawTextureFormat::R8G8B8)
    {
        return 3;
    }
    else
    {
        ASUNA_ASSERT(false);
        return -1;
    }
}

void *RawTexture::GetData()
{
    return m_Data;
}

RawTextureFormat RawTexture::GetFormat() const
{
    return m_Format;
}


Texture::Texture(const std::shared_ptr<RawTexture>& rawImage) :
    m_Width(rawImage->GetWidth()),
    m_Height(rawImage->GetHeight()),
    m_Format(rawImage->GetFormat())
{

}

int Texture::GetPixelBytes()
{
    if (m_Format == RawTextureFormat::R8G8B8A8)
    {
        return 4;
    }
    else if (m_Format == RawTextureFormat::R8G8B8)
    {
        return 3;
    }
    else
    {
        ASUNA_ASSERT(false);
        return -1;
    }
}

int Texture::GetHeight() const
{
    return m_Height;
}

int Texture::GetWidth() const
{
    return m_Width;
}

RawTextureFormat Texture::GetFormat() const
{
    return m_Format;
}
