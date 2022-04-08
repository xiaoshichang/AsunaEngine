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
		RenderTarget(RenderTargetDesc desc)
		{
		}

		virtual ~RenderTarget()
		{
		}

    private:
        RenderTargetDesc m_Desc;

	};

}