#pragma once


namespace asuna
{
    struct RenderTargetDesc
    {
        int width;
        int height;
    };


	class RenderTarget
	{
	public:
        RenderTarget() = delete;
		explicit RenderTarget(RenderTargetDesc desc)
		{
		}

		virtual ~RenderTarget() = default;
        virtual void Resize(int width, int height) = 0;

    private:
        RenderTargetDesc m_Desc{};

	};

}