#include "pch.h"
#include "CppUnitTest.h"
#include "AsunaEngine/Foundation/Math/AMath.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace asuna;

namespace AsunaEngineUnitTest
{
	TEST_CLASS(AsunaEngineMathTest)
	{
	public:
		TEST_METHOD(TestVectorEqule)
		{
			Vector3i v1(1, 1, 1);
			Vector3i v2(1, 1, 1);
			Assert::IsTrue(v1 == v2);
		}


		TEST_METHOD(TestVectorAdd)
		{
			Vector3i v1(1, 1, 1);
			Vector3i v2(2, 2, 2);
			Vector3i v3(3, 3, 3);
			auto v4 = v1 + v2;
			Vector3i v5(1, 1, 1);
			Vector3i v6(2, 2, 2);
			Assert::IsTrue(v4 == v3);
			Assert::IsTrue(v1 == v5);
			Assert::IsTrue(v2 == v6);
		}

		TEST_METHOD(TestVectorSubstraction)
		{
			Vector3i v1(1, 1, 1);
			Vector3i v2(2, 2, 2);
			Vector3i v3(3, 3, 3);
			auto v4 = v3 - v2;
			Vector3i v5(2, 2, 2);
			Vector3i v6(3, 3, 3);
			Assert::IsTrue(v4 == v1);
			Assert::IsTrue(v2 == v5);
			Assert::IsTrue(v3 == v6);
		}

		TEST_METHOD(TestVectorScale)
		{
			Vector3i v3(3, 3, 3);
			auto v4 = v3 * 3;
			auto v5 = 3 * v3;
			Vector3i v6(9, 9, 9);
			Vector3i v7(3, 3, 3);
			Assert::IsTrue(v4 == v5);
			Assert::IsTrue(v5 == v6);
			Assert::IsTrue(v7 == v3);
		}

		TEST_METHOD(TestVectorNormalize)
		{
			Vector3f v1(1, 1, 1);
			auto v2 = v1.NormalizeCopy();
			v1.Normalize();
			Assert::IsTrue(v2 == v1);
			Assert::IsTrue(FloatEquel(1, v2.LengthSquare()));
			Assert::IsTrue(FloatEquel(1, v1.LengthSquare()));
		}

		TEST_METHOD(TestVectorDotProduct)
		{
			Vector3f v1(2, 2, 2);
			Vector3f v2(3, 3, 3);
			Assert::IsTrue(FloatEquel(18, v2.DotProduct(v1)));
		}

		TEST_METHOD(TestVectorCrossProduct)
		{
			//https://mathinsight.org/cross_product_examples
			Vector3f v1(3, -3, 1);
			Vector3f v2(4, 9, 2);
			auto v3 = v1.CrossProduct(v2);
			Vector3f v4(-15, -2, 39);
			Assert::IsTrue(v4 == v3);
		}


		
	};
}
