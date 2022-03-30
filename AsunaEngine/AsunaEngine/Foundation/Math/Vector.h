#pragma once

#include <cstdint>
#include <iostream>
#include <limits>
#include <math.h>
#include <algorithm>
#include "../Platform/Assert.h"
#include "MathCommon.h"


namespace asuna {

	template<typename T, size_t SizeOfArray>
	constexpr size_t ElementCount(T(&)[SizeOfArray]) { return SizeOfArray; }

	template <typename T>
	struct Vector2Type {
		union {
			T data[2];
			struct { T x, y; };
			struct { T r, g; };
			struct { T u, v; };
		};

		Vector2Type<T>() {};
		Vector2Type<T>(const T& _v) : x(_v), y(_v) {};
		Vector2Type<T>(const T& _x, const T& _y) : x(_x), y(_y) {};

		T& operator[](int rowIndex)
		{
			ASUNA_ASSERT(rowIndex < 2);
			return data[rowIndex];
		}

		bool operator==(const Vector2Type<T>& rhs) const
		{
			auto c1 = comparison_traits<T>::equal(x, rhs.x);
			auto c2 = comparison_traits<T>::equal(y, rhs.y);
			return c1 && c2;
		}

		Vector2Type<T> operator+(const Vector2Type<T>& rhs) const
		{
			Vector2Type<T> ret;
			ret.x = x + rhs.x;
			ret.y = y + rhs.y;
			return ret;
		}

		Vector2Type<T> operator-(const Vector2Type<T>& rhs) const
		{
			Vector2Type<T> ret;
			ret.x = x - rhs.x;
			ret.y = y - rhs.y;
			return ret;
		}

		Vector2Type<T> operator*(T scale)  const
		{
			Vector2Type<T> ret;
			ret.x = x * scale;
			ret.y = y * scale;
			return ret;
		}

		T LengthSquare() const
		{
			return x * x + y * y;
		}

		float Length() const
		{
			return sqrt(LengthSquare());
		}

		void Normalize()
		{
			float lengthScale = 1.0 / Length();
			for (size_t i = 0; i < 2; ++i)
			{
				data[i] = data[i] * lengthScale;
			}
		}

		Vector2Type<float> NormalizeCopy() const
		{
			Vector2Type<float> ret;
			float lengthScale = 1.0 / Length();
			for (size_t i = 0; i < 2; ++i)
			{
				ret.data[i] = data[i] * lengthScale;
			}
			return ret;
		}

		T DotProduct(const Vector2Type<T>& lhs) const
		{
			T result = 0;
			for (size_t i = 0; i < 2; ++i)
			{
				result += data[i] * lhs.data[i];
			}
			return result;
		}


	};

	typedef Vector2Type<float> Vector2f;


	template <typename T>
	struct Vector3Type {
		union {
			T data[3];
			struct { T x, y, z; };
			struct { T r, g, b; };
		};

		Vector3Type<T>() {};
		Vector3Type<T>(const T& _v) : x(_v), y(_v), z(_v) {};
		Vector3Type<T>(const T& _x, const T& _y, const T& _z) : x(_x), y(_y), z(_z) {};

		T& operator[](int rowIndex)
		{
			ASUNA_ASSERT(rowIndex < 3);
			return data[rowIndex];
		}

		Vector3Type<T> operator+(const Vector3Type<T>& rhs)	const
		{
			Vector3Type<T> ret;
			ret.x = x + rhs.x;
			ret.y = y + rhs.y;
			ret.z = z + rhs.z;
			return ret;
		}

		Vector3Type<T> operator-(const Vector3Type<T>& rhs) const
		{
			Vector3Type<T> ret;
			ret.x = x - rhs.x;
			ret.y = y - rhs.y;
			ret.z = z - rhs.z;
			return ret;
		}

		Vector3Type<T> operator*(T scale) const
		{
			Vector3Type<T> ret;
			ret.x = x * scale;
			ret.y = y * scale;
			ret.z = z * scale;
			return ret;
		}

		friend Vector3Type<T> operator*(T scale, const Vector3Type<T>& lhs)	
		{
			Vector3Type<T> ret;
			ret.x = lhs.x * scale;
			ret.y = lhs.y * scale;
			ret.z = lhs.z * scale;
			return ret;
		}

		bool operator==(const Vector3Type<T>& rhs) const
		{
			auto c1 = comparison_traits<T>::equal(x, rhs.x);
			auto c2 = comparison_traits<T>::equal(y, rhs.y);
			auto c3 = comparison_traits<T>::equal(z, rhs.z);
			return c1 && c2 && c3;
		}

		T LengthSquare() const
		{
			return x * x + y * y + z * z;
		}

		float Length() const
		{
			return sqrt(LengthSquare());
		}

		Vector3Type<T> CrossProduct(const Vector3Type<T>& lhs) const
		{
			Vector3Type<T> result;
			result[0] = data[1] * lhs.data[2] - data[2] * lhs.data[1];
			result[1] = data[2] * lhs.data[0] - data[0] * lhs.data[2];
			result[2] = data[0] * lhs.data[1] - data[1] * lhs.data[0];
			return result;
		}

		T DotProduct(const Vector3Type<T>& lhs)	const
		{
			T result = 0;
			for (size_t i = 0; i < 3; ++i) 
			{
				result += data[i] * lhs.data[i];
			}
			return result;
		}

		void Normalize()
		{
			float lengthScale = 1.0f / Length();
			for (size_t i = 0; i < 3; ++i)
			{
				data[i] = data[i] * lengthScale;
			}
		}

		Vector3Type<float> NormalizeCopy() const
		{
			Vector3Type<float> ret;
			float lengthScale = 1.0f / Length();
			for (size_t i = 0; i < 3; ++i)
			{
				ret.data[i] = data[i] * lengthScale;
			}
			return ret;
		}

	};

	typedef Vector3Type<float> Vector3f;
	typedef Vector3Type<int> Vector3i;

	template <typename T>
	struct Vector4Type {
		union {
			T data[4];
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
		};

		Vector4Type<T>() {};
		Vector4Type<T>(const T& _v) : x(_v), y(_v), z(_v), w(_v) {};
		Vector4Type<T>(const T& _x, const T& _y, const T& _z, const T& _w) : x(_x), y(_y), z(_z), w(_w) {};
		Vector4Type<T>(const Vector3Type<T>& v3) : x(v3.x), y(v3.y), z(v3.z), w(1.0f) {};
		Vector4Type<T>(const Vector3Type<T>& v3, const T& _w) : x(v3.x), y(v3.y), z(v3.z), w(_w) {};

		T& operator[](int rowIndex)
		{
			ASUNA_ASSERT(rowIndex < 4);
			return data[rowIndex];
		}

		Vector4Type<T> operator+(const Vector4Type<T>& rhs) const
		{
			Vector4Type<T> ret;
			ret.x = x + rhs.x;
			ret.y = y + rhs.y;
			ret.z = z + rhs.z;
			ret.w = w + rhs.w;
			return ret;
		}

		Vector4Type<T> operator-(const Vector4Type<T>& rhs) const
		{
			Vector4Type<T> ret;
			ret.x = x - rhs.x;
			ret.y = y - rhs.y;
			ret.z = z - rhs.z;
			ret.w = w - rhs.w;
			return ret;
		}

		Vector4Type<T> operator*(T scale) const
		{
			Vector4Type<T> ret;
			ret.x = x * scale;
			ret.y = y * scale;
			ret.z = z * scale;
			ret.w = w * scale;
			return ret;
		}

		friend Vector4Type<T> operator*(T scale, const Vector4Type<T>& lhs) 
		{
			Vector4Type<T> ret;
			ret.x = lhs.x * scale;
			ret.y = lhs.y * scale;
			ret.z = lhs.z * scale;
			ret.w = lhs.w * scale;
			return ret;
		}

		bool operator==(const Vector4Type<T>& rhs) const
		{
			auto c1 = comparison_traits<T>::equal(x, rhs.x);
			auto c2 = comparison_traits<T>::equal(y, rhs.y);
			auto c3 = comparison_traits<T>::equal(z, rhs.z);
			auto c4 = comparison_traits<T>::equal(w, rhs.w);
			return c1 && c2 && c3 && c4;
		}

		T LengthSquare() const
		{
			return x * x + y * y + z * z + w * w;
		}

		float Length() const
		{
			return sqrt(LengthSquare());
		}


		void Normalize()
		{
			float lengthScale = 1.0 / Length();
			for (size_t i = 0; i < 4; ++i)
			{
				data[i] = data[i] * lengthScale;
			}
		}

		Vector4Type<float> NormalizeCopy() const
		{
			Vector4Type<float> ret;
			float lengthScale = 1.0 / Length();
			for (size_t i = 0; i < 4; ++i)
			{
				ret.data[i] = data[i] * lengthScale;
			}
			return ret;
		}


		T DotProduct(const Vector4Type<T>& lhs) const
		{
			T result = 0;
			for (size_t i = 0; i < 4; ++i)
			{
				result += data[i] * lhs.data[i];
			}
			return result;
		}

	};

	typedef Vector4Type<float> Vector4f;
	typedef Vector4Type<uint8_t> R8G8B8A8Unorm;
	typedef Vector4Type<int> Vector4i;
	typedef Vector4Type<float> Color;


}