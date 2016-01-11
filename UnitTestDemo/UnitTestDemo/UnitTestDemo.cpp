// UnitTestDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

#include "gtest/gtest.h"

int Foo(int a, int b)
{
	if (a > b)
		return a - b;
	else if (a == b)
		throw a;
	else
		return a + b;
}

//TEST(FooTest, abcd)
//{
//	EXPECT_THROW(Foo(2, 2), int);
//	EXPECT_EQ(2, Foo(5, 3));
//	EXPECT_EQ(8, Foo(3, 5));
//}

TEST(ImageCompareTest, compare)
{
	//CImageCompare imgCompare;
	//ImageContext pImgContext1 = CImageCompare::openImage(L"C:\\test.png");
	//ASSERT_TRUE(pImgContext1 != nullptr);
	//CImageCompare::closeImage(pImgContext1);
}

//bool isEven(unsigned long n)
//{
//	return n % 2 == 0;
//}

//class IsEvenTest : public testing::TestWithParam<unsigned long>
//{
//
//};
//TEST_P(IsEvenTest, TrueReturn)
//{
//	unsigned long n = GetParam();
//	EXPECT_TRUE(isEven(n));
//}
//
//INSTANTIATE_TEST_CASE_P(IsEvenTest, IsEvenTest, testing::Values(2, 4, 6, 8, 10));

int _tmain(int argc, _TCHAR* argv[])
{
	//testing::InitGoogleTest(&argc, argv);
	//int nRet = RUN_ALL_TESTS();

	//CImageCompare imgCompare;
	//ImageContext pImgContext1 = CImageCompare::openImage(L"C:\\test.bmp");
	//ImageContext pImgContext2 = CImageCompare::openImage(L"C:\\test_Difference.bmp");
	//CImageCompare::showImage(pImgContext1);
	//CCompareResult r = CImageCompare::compare(pImgContext1, pImgContext2, CImageCompare::cmp_markDifference);

	//CImageCompare::showImage(pImgContext2);

	//CImageCompare::closeImage(pImgContext2);
	//CImageCompare::closeImage(pImgContext1);

	return 0;
}

