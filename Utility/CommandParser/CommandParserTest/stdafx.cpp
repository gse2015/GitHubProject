// stdafx.cpp : 只包括标准包含文件的源文件
// CommandParserTest.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中引用任何所需的附加头文件，
//而不是在此文件中引用

#ifdef _DEBUG
#pragma comment(lib, "../../../ThirdLibrary/gtest-1.3.0/Lib/gtestd.lib")
#else
#pragma comment(lib, "../../../ThirdLibrary/gtest-1.3.0/Lib/gtest.lib")
#endif