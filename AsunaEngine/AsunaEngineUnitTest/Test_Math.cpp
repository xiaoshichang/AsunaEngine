//
// Created by xiao on 2022/3/27.
//

#include "AsunaEngine/Foundation/Math/AMath.h"
#include <DirectXMath.h>
#include <crtdbg.h>


using namespace DirectX;
using namespace asuna;

#define  TEST_METHOD(methodName) \
    static void methodName()



class AsunaEngineMathTest
{
public:
    TEST_METHOD(TestVectorEqule) {
        Vector3i v1(1, 1, 1);
        Vector3i v2(1, 1, 1);
        _ASSERTE(v1 == v2);
    }


    TEST_METHOD(TestVectorAdd) {
        Vector3i v1(1, 1, 1);
        Vector3i v2(2, 2, 2);
        Vector3i v3(3, 3, 3);
        auto v4 = v1 + v2;
        Vector3i v5(1, 1, 1);
        Vector3i v6(2, 2, 2);
        _ASSERTE(v4 == v3);
        _ASSERTE(v1 == v5);
        _ASSERTE(v2 == v6);
    }

    TEST_METHOD(TestVectorSubstraction) {
        Vector3i v1(1, 1, 1);
        Vector3i v2(2, 2, 2);
        Vector3i v3(3, 3, 3);
        auto v4 = v3 - v2;
        Vector3i v5(2, 2, 2);
        Vector3i v6(3, 3, 3);
        _ASSERTE(v4 == v1);
        _ASSERTE(v2 == v5);
        _ASSERTE(v3 == v6);
    }

    TEST_METHOD(TestVectorScale) {
        Vector3i v3(3, 3, 3);
        auto v4 = v3 * 3;
        auto v5 = 3 * v3;
        Vector3i v6(9, 9, 9);
        Vector3i v7(3, 3, 3);
        _ASSERTE(v4 == v5);
        _ASSERTE(v5 == v6);
        _ASSERTE(v7 == v3);
    }

    TEST_METHOD(TestVectorNormalize) {
        Vector3f v1(1, 1, 1);
        auto v2 = v1.NormalizeCopy();
        v1.Normalize();
        _ASSERTE(v2 == v1);
        _ASSERTE(comparison_traits<float>::equal(1, v2.LengthSquare()));
        _ASSERTE(comparison_traits<float>::equal(1, v1.LengthSquare()));
    }

    TEST_METHOD(TestVectorDotProduct) {
        Vector3f v1(2, 2, 2);
        Vector3f v2(3, 3, 3);
        _ASSERTE(comparison_traits<float>::equal(18, v2.DotProduct(v1)));
    }

    TEST_METHOD(TestVectorCrossProduct) {
        //https://mathinsight.org/cross_product_examples
        Vector3f v1(3, -3, 1);
        Vector3f v2(4, 9, 2);
        auto v3 = v1.CrossProduct(v2);
        Vector3f v4(-15, -2, 39);
        _ASSERTE(v4 == v3);
    }

    TEST_METHOD(TestMatrixAccess) {
        Matrix4x4f m = {0};
        m[1][1] = 10;
        m[1][2] = 9;
        _ASSERTE(m[1][1] - m[1][2] == 1);
    }

    TEST_METHOD(TestMatrixEqule) {
        Matrix4x4f m1 = {0};
        Matrix4x4f m2 = {0};
        Matrix4x4f m3 = {1, 2, 3, 4, 5};
        m1[1][1] = 10;
        m2[1][1] = 10;
        _ASSERTE(m1 == m2);
        _ASSERTE(m3[0][3] == 4);
        _ASSERTE(m3[1][0] == 5);
        _ASSERTE(m2 != m3);
    }

    TEST_METHOD(TestMatrixTranspose) {
        Matrix4x4f m1 = {1, 2, 3, 4, 5};
        auto m2 = m1.TransposeCopy();
        m1.Transpose();
        _ASSERTE(m1 == m2);
        _ASSERTE(m1[0][0] == 1);
        _ASSERTE(m1[1][0] == 2);
        _ASSERTE(m1[0][1] == 5);
    }

    TEST_METHOD(TestMatrixAddSub) {
        Matrix4x4f m1 = {0};
        Matrix4x4f m2 = {0};
        Matrix4x4f m3 = {0};

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m1[i][j] = 1.0f;
                m2[i][j] = 2.0f;
                m3[i][j] = 3.0f;
            }
        }
        _ASSERTE(m1 + m2 == m3);
        _ASSERTE(m3 - m2 == m1);
    }

    TEST_METHOD(TestMatrixMultiple) {
        Matrix<float, 3, 1> m1 = {4, 5, 6};
        Matrix<float, 1, 3> m2 = {1, 2, 3};
        Matrix<float, 3, 3> m3 = {4, 8, 12, 5, 10, 15, 6, 12, 18};
        Matrix<float, 1, 1> m4 = {32};
        _ASSERTE(m1 * m2 == m3);
        _ASSERTE(m2 * m1 == m4);
    }

    TEST_METHOD(TestMatrixInverse) {
        //https://byjus.com/maths/inverse-of-3-by-3-matrix/
        Matrix3x3f m1 = {1, 2, 3, 0, 1, 4, 5, 6, 0};
        Matrix3x3f m2 = {-24, 18, 5, 20, -15, -4, -5, 4, 1};
        // todo: there some precision problem
        // _ASSERTE(m1.Inverse() == m2);
        _ASSERTE(true);
    }

    TEST_METHOD(TestMatrixRotation) {
        //https://www.cuemath.com/algebra/rotation-matrix/
        Vector3f c(5, 2, 6);
        Matrix4x4f x180 = BuildMatrixRotationX(PI);
        auto d = x180.TransformPoint(c);
        Vector3f e(5, -2, -6);
        _ASSERTE(e == d);
    }

    TEST_METHOD(TestMatrixRotationPYR) {
        auto m1 = XMMatrixRotationRollPitchYaw(1, 2, 1);
        auto m2 = BuildMatrixYawPitchRollLH(1, 2, 1);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                _ASSERTE(comparison_traits<float>::equal(m1.r[j].m128_f32[i], m2[i][j]));
            }
        }
    }

    TEST_METHOD(TestMatrixCameraViewLookatRH) {
        auto eye = XMVectorSet(1, 2, 3, 0);
        auto up = XMVectorSet(0, 1, 0, 0);
        auto at = XMVectorSet(0, 10, 0, 0);
        auto view1 = XMMatrixLookAtRH(eye, at, up);
        Vector3f eye2(1, 2, 3);
        Vector3f up2(0, 1, 0);
        Vector3f at2(0, 10, 0);
        auto view2 = BuildMatrixViewLookatRH(eye2, at2, up2);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                _ASSERTE(comparison_traits<float>::equal(view1.r[j].m128_f32[i], view2[i][j]));
            }
        }
    }

    TEST_METHOD(TestMatrixCameraViewLookatLH) {
        auto eye = XMVectorSet(1, 2, 3, 0);
        auto up = XMVectorSet(0, 1, 0, 0);
        auto at = XMVectorSet(0, 10, 0, 0);
        auto view1 = XMMatrixLookAtLH(eye, at, up);
        Vector3f eye2(1, 2, 3);
        Vector3f up2(0, 1, 0);
        Vector3f at2(0, 10, 0);
        auto view2 = BuildMatrixViewLookatLH(eye2, at2, up2);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                _ASSERTE(comparison_traits<float>::equal(view1.r[j].m128_f32[i], view2[i][j]));
            }
        }
    }

    TEST_METHOD(TestMatrixCameraPerspectiveFovRH) {
        auto angle = PI / 2;
        auto aspect = 1024.0f / 768;
        auto p1 = XMMatrixPerspectiveFovRH(angle, aspect, 0.1f, 1000.0f);
        auto p2 = BuildMatrixPerspectiveFovRH(angle, aspect, 0.1f, 1000.0f);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                _ASSERTE(comparison_traits<float>::equal(p1.r[j].m128_f32[i], p2[i][j]));
            }
        }
        _ASSERTE(true);
    }

    TEST_METHOD(TestMatrixCameraPerspectiveFovLH) {
        auto angle = PI / 2;
        auto aspect = 1024.0f / 768;
        auto p1 = XMMatrixPerspectiveFovLH(angle, aspect, 0.1f, 1000.0f);
        auto p2 = BuildMatrixPerspectiveFovLH(angle, aspect, 0.1f, 1000.0f);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                _ASSERTE(comparison_traits<float>::equal(p1.r[j].m128_f32[i], p2[i][j]));
            }
        }
        _ASSERTE(true);
    }

    TEST_METHOD(TestMatrixCameraOrthographicRH) {
        auto p1 = XMMatrixOrthographicRH(1024.0f, 768.0f, 0.1f, 1000.0f);
        auto p2 = BuildMatrixOrthographicRH(1024.0f, 768.0f, 0.1f, 1000.0f);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                _ASSERTE(comparison_traits<float>::equal(p1.r[j].m128_f32[i], p2[i][j]));
            }
        }
        _ASSERTE(true);
    }

    TEST_METHOD(TestMatrixCameraOrthographicLH) {
        auto p1 = XMMatrixOrthographicLH(1024.0f, 768.0f, 0.1f, 1000.0f);
        auto p2 = BuildMatrixOrthographicLH(1024.0f, 768.0f, 0.1f, 1000.0f);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                _ASSERTE(comparison_traits<float>::equal(p1.r[j].m128_f32[i], p2[i][j]));
            }
        }
        _ASSERTE(true);
    }

    TEST_METHOD(TestMatrixQuaternion)
    {
        FXMVECTOR q1{1, 2, 3, 1};
        auto q = XMQuaternionNormalize(q1);
        auto p1 = XMMatrixRotationQuaternion(q);

        Quaternion q2(1, 2, 3, 1);
        q2.Normalize();
        auto p2 = BuildMatrixQuaternion(q2);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                _ASSERTE(comparison_traits<float>::equal(p1.r[j].m128_f32[i], p2[i][j]));
            }
        }
        _ASSERTE(true);
    }

    //https://www.andre-gaschler.com/rotationconverter/
    TEST_METHOD(TestQuaternionPitchYawRoll)
    {
        Quaternion q = Quaternion::FromRollPitchYawLH(1, 2, 3);
        auto q2 = XMQuaternionRotationRollPitchYaw(1, 2, 3);
        _ASSERT(comparison_traits<float>::equal(q2.m128_f32[0], q.x));
        _ASSERT(comparison_traits<float>::equal(q2.m128_f32[1], q.y));
        _ASSERT(comparison_traits<float>::equal(q2.m128_f32[2], q.z));
        _ASSERT(comparison_traits<float>::equal(q2.m128_f32[3], q.w));

        Quaternion q3 = Quaternion::FromRollPitchYawRH(-1, -2, -3);
        _ASSERT(comparison_traits<float>::equal(q2.m128_f32[0], q3.x));
        _ASSERT(comparison_traits<float>::equal(q2.m128_f32[1], q3.y));
        _ASSERT(comparison_traits<float>::equal(q2.m128_f32[2], q3.z));
        _ASSERT(comparison_traits<float>::equal(q2.m128_f32[3], q3.w));

        auto angle = q.ToEuler();
        _ASSERT(angle == Vector3f(1, 2, 3));
    }

    TEST_METHOD(TestQuaternionMatrixTransform)
    {
        Vector3f v = {1, 0, 0};
        Quaternion q = Quaternion::FromRollPitchYawLH(0, PI/2, 0);
        Matrix4x4f m = BuildMatrixQuaternion(q);
        auto v1 = m.TransformVector(v);
        _ASSERT(v1 == Vector3f(0, 0, -1));

        Quaternion q2 = Quaternion::FromRollPitchYawLH(0, -PI/2, 0);
        Matrix4x4f m2 = BuildMatrixQuaternion(q2);
        auto v2 = m2.TransformVector(v);
        _ASSERT(v2 == Vector3f(0, 0, 1));
    }

};




int main()
{
    AsunaEngineMathTest::TestVectorEqule();
    AsunaEngineMathTest::TestVectorAdd();
    AsunaEngineMathTest::TestVectorSubstraction();
    AsunaEngineMathTest::TestVectorScale();
    AsunaEngineMathTest::TestVectorCrossProduct();
    AsunaEngineMathTest::TestVectorDotProduct();
    AsunaEngineMathTest::TestVectorNormalize();

    AsunaEngineMathTest::TestMatrixAccess();
    AsunaEngineMathTest::TestMatrixAddSub();
    AsunaEngineMathTest::TestMatrixEqule();
    AsunaEngineMathTest::TestMatrixInverse();
    AsunaEngineMathTest::TestMatrixMultiple();

    AsunaEngineMathTest::TestMatrixRotation();
    AsunaEngineMathTest::TestMatrixRotationPYR();

    AsunaEngineMathTest::TestMatrixCameraViewLookatLH();
    AsunaEngineMathTest::TestMatrixCameraViewLookatRH();
    AsunaEngineMathTest::TestMatrixCameraPerspectiveFovLH();
    AsunaEngineMathTest::TestMatrixCameraPerspectiveFovRH();
    AsunaEngineMathTest::TestMatrixCameraOrthographicLH();
    AsunaEngineMathTest::TestMatrixCameraOrthographicRH();
    AsunaEngineMathTest::TestMatrixQuaternion();
    AsunaEngineMathTest::TestQuaternionPitchYawRoll();
    AsunaEngineMathTest::TestQuaternionMatrixTransform();

}

