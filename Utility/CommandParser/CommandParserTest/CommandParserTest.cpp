// CommandParserTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../CommandParser.h"


int main(int argc, char * argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	//auto pCmdParser = CreateCommandParser();
	//ICommandParser::OptionItem cmpOption[] = 
	//{
	//	  {'h', ICommandParser::no,		"help"		}
	//	, {'o', ICommandParser::yes,	"old"		}
	//	, {'n', ICommandParser::yes,	"new"		}
	//	, {'r', ICommandParser::yes,	"result"	}
	//	, {'\0', ICommandParser::yes,	nullptr		}
	//};
	//pCmdParser->addOpt(cmpOption, "compare");
	//pCmdParser->addOpt(cmpOption, "show");

	////const char* const myArgv[] = { "test.exe", "compare", "-h" };
	//const char* const myArgv[] = { "test.exe", "compare", "-o", "C:\\oldVersion", "-n", "C:\\newVersion", "-r", "C:\\result" };
	//bool bRet = pCmdParser->parseCommand(_countof(myArgv), myArgv);

	//auto lpszCmdName = pCmdParser->getCommand();
	//auto nOptCount = pCmdParser->getOptCount();
	//for (int i = 0; i < nOptCount; i++)
	//{
	//	auto opt = pCmdParser->getOpt(i);
	//	auto pArg = pCmdParser->getOptArg(i);
	//	char* aa = "";
	//}
    return 0;
}

