//
// Created by xiao on 2022/3/30.
//

#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace asuna
{

    inline void threeaxisrot(float r11, float r12, float r21, float r31, float r32, float res[])
    {
        res[0] = atan2( r31, r32 );
        r21 = std::clamp(r21, -1.0f, 1.0f);
        res[1] = asin ( r21 );
        res[2] = atan2( r11, r12 );
    }

    struct Quaternion
    {
        float x, y, z, w;

        Quaternion()
        {
            this->x = 0;
            this->y = 0;
            this->z = 0;
            this->w = 1;
        }

        Quaternion(float x, float y, float z, float w)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        static Quaternion Identity()
        {
            return {0, 0, 0, 1};
        }


        float operator+(const Quaternion& rhs) const
        {
            Vector3f v1(x, y, z);
            Vector3f v2(rhs.x, rhs.y, rhs.z);
            return w * rhs.w + v1.DotProduct(v2);
        }

        bool operator==(const Quaternion& rhs) const
        {
            auto c1 = comparison_traits<float>::equal(x, rhs.x);
            auto c2 = comparison_traits<float>::equal(y, rhs.y);
            auto c3 = comparison_traits<float>::equal(z, rhs.z);
            auto c4 = comparison_traits<float>::equal(w, rhs.w);
            return c1 && c2 && c3 && c4;
        }

        bool operator!=(const Quaternion& rhs) const
        {
            auto ret = *this == rhs;
            return !ret;
        }

        Quaternion operator*(const Quaternion& rhs) const
        {
            Vector3f v1(x, y, z);
            Vector3f v2(rhs.x, rhs.y, rhs.z);
            Vector3f xyz = v1.CrossProduct(v2) + v1 * rhs.w + v2 * w;
            float real = w * rhs.w - v1.DotProduct(v2);
            return {real, xyz.x, xyz.y, xyz.z};
        }

        void Normalize()
        {
            Vector4f v(x, y, z, w);
            v.Normalize();
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
        }

        Quaternion NormalizeCopy() const
        {
            Vector4f v(x, y, z, w);
            v.Normalize();
            return {v.x, v.y, v.z, v.w};
        }

        Quaternion Inverse() const
        {
            Quaternion Result;
            float LengthSq = x*x + y*y + z*z + w*w;
            Result.x = -x / LengthSq;
            Result.y = -y / LengthSq;
            Result.z = -z / LengthSq;
            Result.w = w / LengthSq;
            return Result;
        }


        // rotations are performed around the Z axis, the X axis, and the Y axis, in that order.
        // http://www.andre-gaschler.com/rotationconverter/
        // https://www.cnblogs.com/21207-ihome/p/6894128.html
        // enum RotSeq{zyx, zyz, zxy, zxz, yxz, yxy, yzx, yzy, xyz, xyx, xzy,xzx};
        // using yxz, result is ZXY, so return res[1], res[2], res[0]
        Vector3f ToEuler()
        {
            Quaternion q = *this;
            float res[3];
            threeaxisrot( 2*(q.x*q.z + q.w*q.y),
                          q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                          -2*(q.y*q.z - q.w*q.x),
                          2*(q.x*q.y + q.w*q.z),
                          q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                          res);
            return {res[1], res[2], res[0]};
        }

        // https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmquaternionrotationrollpitchyaw
        // The order of transformations is roll(z) first, then pitch(x), then yaw(y).
        static Quaternion FromRollPitchYawLH(float pitch, float yaw, float roll)
        {
            const float halfpitch = pitch * 0.5f;
            float cp = cosf(halfpitch);
            float sp = sinf(halfpitch);

            const float halfyaw = yaw * 0.5f;
            float cy = cosf(halfyaw);
            float sy = sinf(halfyaw);

            const float halfroll = roll * 0.5f;
            float cr = cosf(halfroll);
            float sr = sinf(halfroll);

            float x = cr * sp * cy + sr * cp * sy;
            float y = cr * cp * sy - sr * sp * cy;
            float z = sr * cp * cy - cr * sp * sy;
            float w = cr * cp * cy + sr * sp * sy;
            return Quaternion(x, y, z, w);
        }

        // https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmquaternionrotationrollpitchyaw
        // To use right-handed coordinates, negate all three angles.
        static Quaternion FromRollPitchYawRH(float pitch, float yaw, float roll)
        {
            return FromRollPitchYawLH(-pitch, -yaw, -roll);
        }

    };


    /// Build rotation matrix from a quaternion
    /// note that transform from quaternion to rotation matrix does not involve handedness of any sort.
    /// see <<Derivation of Equations>> part of http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/
    /// P2 = q * P1 * q'
    /// P2 = [M] P1
    /// It is purely solve for the matrix M such that [M] P1 = q * P1 * q'
    /// https://stackoverflow.com/questions/1274936/flipping-a-quaternion-from-right-to-left-handed-coordinates
    inline Matrix4x4f BuildMatrixQuaternion(const Quaternion& q)
    {
        float qx = q.x;
        float qxx = qx * qx;
        float qy = q.y;
        float qyy = qy * qy;
        float qz = q.z;
        float qzz = qz * qz;
        float qw = q.w;
        Matrix4x4f m =
        {{{
          {1.f - 2.f * qyy - 2.f * qzz,       2.f * qx * qy - 2.f * qz * qw,      2.f * qx * qz + 2.f * qy * qw,      0.0f},
          {2.f * qx * qy + 2.f * qz * qw,     1.f - 2.f * qxx - 2.f * qzz,        2.f * qy * qz - 2.f * qx * qw,      0.0f},
          {2.f * qx * qz - 2.f * qy * qw,     2.f * qy * qz + 2.f * qx * qw,      1.f - 2.f * qxx - 2.f * qyy,        0.0f},
          {0.0f,                              0.0f,                               0.f,                                1.0f}
        }}};
        return m;
    }

}
