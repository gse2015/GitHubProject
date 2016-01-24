#include "stdafx.h"
#include "../CommandParser.h"
#include <string>

class CCmdParserTest : public testing::Test
{
protected:
	static void SetUpTestCase()
	{
		s_pCmdParser = CreateCommandParser();
	}
	static void TearDownTestCase()
	{
	}
	static ICommandParser*	s_pCmdParser;

	//virtual void SetUp()
	//{

	//}
	virtual void TearDown()
	{
		ASSERT_TRUE(s_pCmdParser != nullptr);
		s_pCmdParser->resetStatu();
	}
};

ICommandParser* CCmdParserTest::s_pCmdParser = nullptr;

TEST_F(CCmdParserTest, compare)
{
	ICommandParser::OptionItem cmpOption[] =
	{
		 { 'h', ICommandParser::no,		"help" }
		,{ 'o', ICommandParser::yes,	"old" }
		,{ 'n', ICommandParser::yes,	"new" }
		,{ 'r', ICommandParser::yes,	"result" }
		,{ '\0', ICommandParser::yes,	nullptr }
	};
	s_pCmdParser->addOpt(cmpOption, "compare");

	const char* const myArgv[] = { "test.exe", "compare", "-o", "C:\\oldVersion", "-n", "C:\\newVersion", "-r", "C:\\result" };
	bool bRet = s_pCmdParser->parseCommand(_countof(myArgv), myArgv);
	ASSERT_TRUE(bRet);

	auto lpszCmdName = s_pCmdParser->getCommand();
	//ASSERT_STREQ(std::string(lpszCmdName), std::string("compare"));
	ASSERT_STREQ("compare", lpszCmdName);
	auto nOptCount = s_pCmdParser->getOptCount();
	ASSERT_EQ(3, nOptCount);
	char chOpt[] = { 'o', 'n', 'r' };
	char* chArg[] = { "C:\\oldVersion" , "C:\\newVersion", "C:\\result" };
	for (int i = 0; i < nOptCount; i++)
	{
		auto opt = s_pCmdParser->getOpt(i);
		ASSERT_EQ(chOpt[i], opt);
		auto pArg = s_pCmdParser->getOptArg(i);
		ASSERT_EQ(chArg[i], pArg);
	}
}


