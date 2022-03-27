#pragma once

#include <Windows.h>

namespace asuna
{
	enum class RenderSurfaceType
	{
		Unknown,
		WindowsApplication,
		WPFImage,
	};


	struct RenderSurface
	{
		RenderSurfaceType Type;
		HWND HWND;
	};
}