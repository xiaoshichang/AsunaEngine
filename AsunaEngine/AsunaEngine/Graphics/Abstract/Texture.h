#pragma once
#include <memory>
#include <string>


namespace asuna
{
    enum class RawTextureFormat
    {
        R8G8B8A8,
        R8G8B8
    };

    class RawTexture
    {
    public:
        explicit RawTexture(int width, int height, RawTextureFormat format, void* data);
        ~RawTexture();

        int GetWidth() const;
        int GetHeight() const;
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
		Texture()
		{
		}

		virtual ~Texture()
		{
		}
	};


}