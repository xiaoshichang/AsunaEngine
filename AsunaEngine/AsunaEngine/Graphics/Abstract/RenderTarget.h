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
            : m_Desc(desc)
		{
		}

		virtual ~RenderTarget() = default;
        virtual void Resize(int width, int height);

    protected:
        RenderTargetDesc m_Desc;

	};

}