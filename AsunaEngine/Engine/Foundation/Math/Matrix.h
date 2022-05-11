#pragma once

#include <cstdint>
#include <iostream>
#include <limits>
#include <math.h>
#include <algorithm>
#include "Vector.h"
#include "../Platform/Assert.h"
#include "MathCommon.h"
#include "Vector.h"


namespace asuna {

	template<typename T, size_t RowSize, size_t ColSize>
	constexpr size_t ElementCount(T(&)[RowSize][ColSize]) { return RowSize * ColSize; }

	template <typename T, int ROWS, int COLS>
	struct Matrix 
	{
		union
		{
			T data[ROWS][COLS];
		};

		operator T* () {
			return &data[0][0];
		}

		T* operator[](int rowIndex) 
		{
			return data[rowIndex];
		}

		const T* operator[](int rowIndex) const 
		{
			return data[rowIndex];
		}


		Matrix<T, ROWS, COLS>& operator=(const T* _data)
		{
			memcpy(data, _data, ROWS * COLS * sizeof(T));
			return *this;
		}

		bool operator==(const Matrix<T, ROWS, COLS>& m) const
		{
			for (int r = 0; r < ROWS; ++r) 
			{
				for (int c = 0; c < COLS; ++c) 
				{
					if (!comparison_traits<T>::equal(data[r][c], m[r][c]))
					{
						return false;
					}
				}
			}
			return true;
		}

		bool operator!=(const Matrix<T, ROWS, COLS>& m) 
		{
			for (int r = 0; r < ROWS; ++r) 
			{
				for (int c = 0; c < COLS; ++c) 
				{
					if (data[r][c] != m[r][c])
					{
						return true;
					}
				}
			}
			return false;
		}

		void Transpose() 
		{
			ASUNA_ASSERT(ROWS == COLS);
			Matrix<T, ROWS, COLS> temp = *this;
			for (int r = 0; r < ROWS; ++r) 
			{
				for (int c = 0; c < COLS; ++c) 
				{
					data[c][r] = temp[r][c];
				}
			}
		}

		static Matrix<T, ROWS, COLS> Identity()
		{
			ASUNA_ASSERT(ROWS == COLS);
			Matrix<T, ROWS, COLS> ret = { 0 };
			for (int r = 0; r < ROWS; ++r)
			{
				ret[r][r] = 1;
			}
			return ret;
		}


		Matrix<T, ROWS, COLS> TransposeCopy() const
		{
			ASUNA_ASSERT(ROWS == COLS);
			Matrix<T, ROWS, COLS> ret;
			for (int r = 0; r < ROWS; ++r) 
			{
				for (int c = 0; c < COLS; ++c) 
				{
					ret[c][r] = data[r][c];
				}
			}
			return ret;
		}

		Matrix<T, ROWS, COLS> operator+(const Matrix<T, ROWS, COLS>& rhs) const
		{
			Matrix<T, ROWS, COLS> ret;
			for (int r = 0; r < ROWS; ++r) 
			{
				for (int c = 0; c < COLS; ++c) 
				{
					ret[r][c] = data[r][c] + rhs[r][c];
				}
			}
			return ret;
		}

		Matrix<T, ROWS, COLS> operator-(const Matrix<T, ROWS, COLS>& rhs) const
		{
			Matrix<T, ROWS, COLS> ret;
			for (int r = 0; r < ROWS; ++r)
			{
				for (int c = 0; c < COLS; ++c)
				{
					ret[r][c] = data[r][c] - rhs[r][c];
				}
			}
			return ret;
		}

		template <int NEWCOLS>
		Matrix<T, ROWS, NEWCOLS>  operator*(const Matrix<T, COLS, NEWCOLS>& rhs) const
		{
			Matrix<T, ROWS, NEWCOLS> tmp;
			for (int a = 0; a < ROWS; ++a) 
			{
				for (int c = 0; c < NEWCOLS; ++c)
				{
					T sum = 0;
					for (int b = 0; b < COLS; ++b)
					{
						sum += data[a][b] * rhs[b][c];
					}
					tmp[a][c] = sum;
				}
			}
			return tmp;
		}


		Matrix<T, ROWS, COLS> Inverse()
		{
			ASUNA_ASSERT(ROWS == COLS);
			Matrix<T, ROWS, COLS> out;
			Matrix<T, ROWS, COLS> in = *this;
			float fDet = 1.0f;
			int f = 1;
			int is[ROWS];
			int js[ROWS];
			int k, i, j, l;

			for (k = 0; k < ROWS; k++)
			{
				float fMax = 0.0f;
				for (i = k; i < ROWS; i++)
				{
					for (j = k; j < ROWS; j++)
					{
						const float f = abs(in[i][j]);
						if (f > fMax)
						{
							fMax = f;
							is[k] = i;
							js[k] = j;
						}
					}
				}
				if (abs(fMax) < 0.00000001f)
				{
					ASUNA_ASSERT(false);
				}
				if (is[k] != k)
				{
					f = -f;
					for (l = 0; l < ROWS; ++l) {
						T temp = in[k][l];
						in[k][l] = in[is[k]][l];
						in[is[k]][l] = temp;
					}
				}
				if (js[k] != k)
				{
					f = -f;
					for (l = 0; l < ROWS; ++l) {
						T temp = in[l][k];
						in[l][k] = in[l][js[k]];
						in[l][js[k]] = temp;
					}
				}
				fDet *= in[k][k];
				in[k][k] = 1.0f / in[k][k];

				for (j = 0; j < ROWS; j++)
				{
					if (j != k)
						in[k][j] *= in[k][k];
				}

				for (i = 0; i < ROWS; i++)
				{
					if (i != k)
					{
						for (j = 0; j < ROWS; j++)
						{
							if (j != k)
								in[i][j] = in[i][j] - in[i][k] * in[k][j];
						}
					}
				}
				for (i = 0; i < ROWS; i++)
				{
					if (i != k)
						in[i][k] *= -in[k][k];
				}
			}
			for (k = ROWS - 1; k >= 0; k--)
			{
				if (js[k] != k)
				{
					for (l = 0; l < ROWS; ++l) {
						T temp = in[k][l];
						in[k][l] = in[js[k]][l];
						in[js[k]][l] = temp;
					}
				}
				if (is[k] != k)
				{
					for (l = 0; l < ROWS; ++l) {
						T temp = in[l][k];
						in[l][k] = in[l][is[k]];
						in[l][is[k]] = temp;
					}
				}
			}
			out = in;
			return out;
		}


		inline Vector3f TransformPoint(const Vector3f& point) const
		{
			ASUNA_ASSERT(COLS == 4);
			ASUNA_ASSERT(ROWS == 4);

			Vector4f hSpace = Vector4f(point.x, point.y, point.z, 1);
			Vector4f tmp = Vector4f(0, 0, 0, 0);

			for (int r = 0; r < 4; ++r)
			{
				for (int c = 0; c < 4; ++c)
				{
					tmp[r] += hSpace[c] * data[r][c];
				}
			}
			return Vector3f(tmp.x, tmp.y, tmp.z);
		}

		inline Vector3f TransformVector(const Vector3f& vector) const
		{
			ASUNA_ASSERT(COLS == 4);
			ASUNA_ASSERT(ROWS == 4);

			Vector4f hSpace = Vector4f(vector.x, vector.y, vector.z, 0);
			Vector4f tmp = Vector4f(0, 0, 0, 0);

			for (int r = 0; r < 4; ++r)
			{
				for (int c = 0; c < 4; ++c)
				{
					tmp[r] += hSpace[c] * data[r][c];
				}
			}
			return Vector3f(tmp.x, tmp.y, tmp.z);
		}

	};

	typedef Matrix<float, 4, 4> Matrix4x4f;
	typedef Matrix<float, 3, 3> Matrix3x3f;


	inline Matrix4x4f BuildMatrixTranslation(const float x, const float y, const float z)
	{
		Matrix4x4f translation = { { {
			{ 1.0f, 0.0f, 0.0f,    x },
			{ 0.0f, 1.0f, 0.0f,    y },
			{ 0.0f, 0.0f, 1.0f,    z },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			} } };
		return translation;
	}

	inline Matrix4x4f BuildMatrixScale(const float x, const float y, const float z)
	{
		Matrix4x4f scale = { { {
			{    x, 0.0f, 0.0f, 0.0f },
			{ 0.0f,    y, 0.0f, 0.0f },
			{ 0.0f, 0.0f,    z, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
			} } };
		return scale;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////  Rotation ////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// https://docs.microsoft.com/en-us/windows/win32/api/_dxmath/
    // pitch to the x-axis, yaw to the y-axis, and roll to the z-axis is a common one for computer graphics and games

	/// <summary>
	/// Builds a matrix that rotates around the x-axis.
	/// </summary>
	/// <param name="radio">Angle of rotation around the x-axis, in radians. Angles are measured clockwise when looking along the rotation axis toward the origin.</param>
	inline Matrix4x4f BuildMatrixRotationX(const float radio) 
	{
		float c = cosf(radio), s = sinf(radio);

		Matrix4x4f tmp = { { {
			{ 1.0f,	0.0f, 0.0f, 0.0f },
			{ 0.0f,	  c,   -s, 0.0f },
			{ 0.0f,   s,   c, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			} } };
		return tmp;
	}

	/// <summary>
	/// Builds a matrix that rotates around the y-axis.
	/// </summary>
	/// <param name="radio">Angle of rotation around the x-axis, in radians. Angles are measured clockwise when looking along the rotation axis toward the origin.</param>
	inline Matrix4x4f BuildMatrixRotationY(const float radio)
	{
		float c = cosf(radio), s = sinf(radio);
		Matrix4x4f tmp = { { {
			{	 c,	0.0f,    s, 0.0f },
			{ 0.0f,	1.0f, 0.0f, 0.0f },
			{	-s, 0.0f,    c, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			} } };
		return tmp;
	}

	/// <summary>
	/// Builds a matrix that rotates around the z-axis.
	/// </summary>
	/// <param name="radio">Angle of rotation around the x-axis, in radians. Angles are measured clockwise when looking along the rotation axis toward the origin.</param>
	inline Matrix4x4f BuildMatrixRotationZ(const float radio) 
	{
		float c = cosf(radio), s = sinf(radio);
		Matrix4x4f tmp = { { {
			{	 c,	  -s, 0.0f, 0.0f },
			{	s,	  c, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			} } };
		return tmp;
	}

	/// <summary>
	/// The assignment of the labels pitch to the x-axis, yaw to the y-axis, and roll to the z-axis 
	/// is a common one for computer graphics and games, since it matches typical 'view' coordinate systems. 
	/// </summary>
	/// <param name="pitch"></param>
	/// <param name="yaw"></param>
	/// <param name="roll"></param>
	inline Matrix4x4f BuildMatrixYawPitchRollLH(const float pitch, const float yaw, const float roll)
	{
		float cp = cosf(pitch);
		float sp = sinf(pitch);
		float cy = cosf(yaw);
		float sy = sinf(yaw);
		float cr = cosf(roll);
		float sr = sinf(roll);

		Matrix4x4f M;
		M.data[0][0] = cr * cy + sr * sp * sy;
		M.data[0][1] = sr * cp;
		M.data[0][2] = sr * sp * cy - cr * sy;
		M.data[0][3] = 0.0f;

		M.data[1][0] = cr * sp * sy - sr * cy;
		M.data[1][1] = cr * cp;
		M.data[1][2] = sr * sy + cr * sp * cy;
		M.data[1][3] = 0.0f;

		M.data[2][0] = cp * sy;
		M.data[2][1] = -sp;
		M.data[2][2] = cp * cy;
		M.data[2][3] = 0.0f;

		M.data[3][0] = 0.0f;
		M.data[3][1] = 0.0f;
		M.data[3][2] = 0.0f;
		M.data[3][3] = 1.0f;
		M.Transpose();
		return M;
	}
	
	
	/// <summary>
	/// Right hand version of BuildMatrixYawPitchRoll
	/// </summary>
	/// <param name="pitch"></param>
	/// <param name="yaw"></param>
	/// <param name="roll"></param>
	inline Matrix4x4f BuildMatrixYawPitchRollRH(const float pitch, const float yaw, const float roll)
	{
		auto m = BuildMatrixYawPitchRollLH(-pitch, -yaw, -roll);
		return m;
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////// Camera Matrix ///////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	inline Matrix4x4f __BuildMatrixViewLookatLH(const Vector3f& EyePosition, const Vector3f& EyeDirection, const Vector3f& UpDirection)
	{
		auto R2 = EyeDirection.NormalizeCopy();
		auto R0 = UpDirection.CrossProduct(R2);
		R0.Normalize();
		auto R1 = R2.CrossProduct(R0);
		auto NegEyePosition = EyePosition * -1;
		auto D0 = R0.DotProduct(NegEyePosition);
		auto D1 = R1.DotProduct(NegEyePosition);
		auto D2 = R2.DotProduct(NegEyePosition);

		Matrix4x4f M = { {{
			{R0.x,	R0.y, R0.z, D0},
			{R1.x,	R1.y, R1.z, D1},
			{R2.x,	R2.y, R2.z, D2},
			{ 0,	0,	  0,  1.0f}
		}} };
		return M;

	}

	inline Matrix4x4f BuildMatrixViewLookatRH(const Vector3f& EyePosition, const Vector3f& FocusPosition, const Vector3f& UpDirection) {
		auto NegEyeDirection = EyePosition - FocusPosition;
		return __BuildMatrixViewLookatLH(EyePosition, NegEyeDirection, UpDirection);
	}

	/// <summary>
	/// Builds a view matrix for a left-handed coordinate system using a camera position, an up direction, and a focal point.
    /// https://blog.csdn.net/silangquan/article/details/50987196
	/// </summary>
	/// <param name="EyePosition"> Position of the camera. </param>
	/// <param name="FocusPosition"> Position of the focal point. </param>
	/// <param name="UpDirection"> Up direction of the camera, typically < 0.0f, 1.0f, 0.0f >. </param>
	inline Matrix4x4f BuildMatrixViewLookatLH(const Vector3f& EyePosition, const Vector3f& FocusPosition, const Vector3f& UpDirection) {
		auto EyeDirection = FocusPosition - EyePosition;
		return __BuildMatrixViewLookatLH(EyePosition, EyeDirection, UpDirection);
	}

	/// <summary>
	/// https://github.com/g-truc/glm/blob/b3f87720261d623986f164b2a7f6a0a938430271/glm/ext/matrix_clip_space.inl
	/// left hand eye space -> left hand clip space.
	/// z range map to [0, 1]
	/// </summary>
	inline Matrix4x4f BuildMatrixPerspectiveFovDX(const float FovAngleY, const float AspectRatio, const float NearZ, const float FarZ) {
        float tanHalfFovY = tanf(FovAngleY / 2.0f);
        Matrix4x4f matrix{};
        matrix[0][0] = 1.0f / (AspectRatio * tanHalfFovY);
        matrix[1][1] = 1.0f / tanHalfFovY;
        matrix[2][2] = FarZ / (FarZ - NearZ);
        matrix[3][2] = 1.0f;
        matrix[2][3] = - (FarZ * NearZ) / (FarZ - NearZ);
		return matrix;
	}

	/// <summary>
	/// left hand eye space -> left hand clip space
	/// z range map to [-1, 1]
	/// </summary>
	inline Matrix4x4f BuildMatrixPerspectiveFovGL(const float FovAngleY, const float AspectRatio, const float NearZ, const float FarZ) {
        float tanHalfFovY = tanf(FovAngleY / 2.0f);
		Matrix4x4f matrix{};
        matrix[0][0] = 1.0f / (AspectRatio * tanHalfFovY);
        matrix[1][1] = 1.0f / tanHalfFovY;
        matrix[2][2] = FarZ / (FarZ - NearZ);
        matrix[3][2] = 1.0f;
        matrix[2][3] = - (2 * FarZ * NearZ) / (FarZ - NearZ);
		return matrix;
	}


    /// <summary>
    /// left hand eye space -> left hand clip space.
    /// z range map to [0, 1]
    /// </summary>
	inline Matrix4x4f BuildMatrixOrthographicDX(const float ViewWidth, const float ViewHeight, const float NearZ, const float FarZ) {

		float fRange = 1.0f / (FarZ - NearZ);
		Matrix4x4f matrix = { {{
		{ 2.0f / ViewWidth,	0.0f,				0.0f,				0.0f },
		{ 0.0f,				2.0f / ViewHeight,	0.0f,				0.0f },
		{ 0.0f,				0.0f,				fRange,				-NearZ * fRange},
		{ 0.0f,				0.0f,				0.0f,				1.0f }
		}} };
		return matrix;
	}

    /// <summary>
    /// left hand eye space -> left hand clip space
    /// z range map to [-1, 1]
    /// </summary>
	inline Matrix4x4f BuildMatrixOrthographicGL(const float ViewWidth, const float ViewHeight, const float NearZ, const float FarZ) {
        float fRange = 1.0f / (FarZ - NearZ);
		Matrix4x4f matrix = { {{
		{ 2.0f / ViewWidth,	0.0f,				0.0f,				0.0f },
		{ 0.0f,				2.0f / ViewHeight,	0.0f,				0.0f },
		{ 0.0f,				0.0f,				fRange,				-(FarZ + NearZ) * fRange},
		{ 0.0f,				0.0f,				0.0f,				1.0f }
		}} };
		return matrix;
	}

}