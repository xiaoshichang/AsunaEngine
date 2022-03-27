#pragma once

#include <cstdint>
#include <iostream>
#include <limits>
#include <math.h>
#include <algorithm>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifndef TWO_PI
#define TWO_PI 3.14159265358979323846f * 2.0f
#endif


namespace asuna {

	//https://stackoverflow.com/questions/27228813/comparing-in-templates
	template <typename T> 
	struct comparison_traits 
	{
		static bool equal(const T& a, const T& b) 
		{
			return a == b;
		}
	};

	template<> struct comparison_traits<float> 
	{
		static bool equal(const float& a, const float& b) 
		{
			return fabs(a - b) < 1e-6; 
		}
	};

	template<> struct comparison_traits<double> 
	{
		static bool equal(const double& a, const double& b) 
		{
			return fabs(a - b) < 1e-15;
		}
	};


	inline float sat(float a) {
		if (a < 0.0f)
		{
			return 0.0f;
		}

		if (a > 1.0f)
		{
			return 1.0f;
		}
		return a;

	}

	inline float Clamp(float v, float a, float b)
	{
		if (v < a)
		{
			return a;
		}

		if (v > b)
		{
			return b;
		}

		return v;
	}


}