#pragma once
#include <memory>
#include <string>
#include <utility>
#include "MaterialParameter.h"

namespace asuna
{
    enum class RawTextureFormat
    {
        R8G8B8A8,
        R8G8B8
    };

    enum class TextureType
    {
        ImageTexture,
        RTTexture
    };

    class RawTexture
    {
    public:
        explicit RawTexture(int width, int height, RawTextureFormat format, void* data);
        ~RawTexture();

        int GetWidth() const;
        int GetHeight() const;
        RawTextureFormat GetFormat() const;
        int GetPixelBytes();
        void* GetData();

    private:
        int m_Width;
        int m_Height;
        RawTextureFormat m_Format;
        unsigned char* m_Data;
    };


	class Texture
	{
	public:
        Texture(TextureType tt);
        TextureType GetTextureType();
		virtual ~Texture() = default;
    private:
        TextureType m_TextureType;
	};

    class ImageTexture : public Texture
    {
    public:
        ImageTexture(const std::shared_ptr<RawTexture>& rawImage);
        int GetWidth() const;
        int GetHeight() const;
        int GetPixelBytes();
        RawTextureFormat GetFormat() const;
    private:
        int m_Width;
        int m_Height;
        RawTextureFormat m_Format;
    };

    class RTTexture : public Texture
    {
    public:
        RTTexture() : Texture(TextureType::RTTexture) {}
    };


}