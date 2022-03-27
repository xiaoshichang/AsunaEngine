#pragma once
#include "Platform.h"
#include "BuildType.h"


// assert implemetation
#if defined(ASUNA_PLATFORM_WINDOWS)
	#include <crtdbg.h>
	#define SCARLETT_ASSERT_IMPL(EXPR)		_ASSERTE(EXPR)
#elif defined(ASUNA_PLATFORM_MACOS)
	#include <assert.h>
	#define SCARLETT_ASSERT_IMPL(EXPR)      assert(EXPR)
#else
#endif

// assert switch
#if defined(ASUNA_BUILD_DEBUG)
	#define ASUNA_ASSERT(EXPR)		SCARLETT_ASSERT_IMPL(EXPR)
#elif defined(ASUNA_BUILD_RELEASE)
	#define ASUNA_ASSERT(EXPR)		((void)0)
#else
    #define ASUNA_ASSERT(EXPR)		((void)0)
#endif // ASUNA_PLAT
