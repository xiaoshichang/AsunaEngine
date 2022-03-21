#pragma once

#if defined(_WIN32) || defined(_WIN64)
	#define ASUNA_PLATFORM_WINDOWS
#elif defined(__APPLE__)
	#define ASUNA_PLATFORM_MACOS
#endif

#if defined(ASUNA_PLATFORM_WINDOWS)
	#include "WindowsOnly.h"
#elif defined(ASUNA_PLATFORM_MACOS)
	#include "MacosOnly.h"
#else
#endif