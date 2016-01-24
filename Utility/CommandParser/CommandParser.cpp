#include "CommandParser.h"
#include <assert.h>
#include <vector>
#include <map>
#include <string>
#include "../ScopeGuard/ScopeGuard.h"

class CCommandParser : public ICommandParser
{
public:
	CCommandParser();
	virtual ~CCommandParser();
	//virtual void		addOpt(const OptionItem& optItem, const char* lpszCmdName);
	virtual void		addOpt(const OptionItem* pOptItems, const char* lpszCmdName);
	virtual bool		parseCommand(const int argc, const char * const argv[]);
	virtual const char*	getCommand()const;
	virtual int			getOptCount()const;
	virtual Option		getOpt(const int& nOptIndex)const;
	virtual const char*	getOptArg(const int& nOptIndex)const;
#ifdef FOR_TEST
	virtual void		resetStatu();
#endif
private:
	struct CommandRecord
	{
		Option		nOpt;
		const char*	lpszArg;
		CommandRecord(const Option& opt = 0, const char*const parg = nullptr)
			: nOpt(opt)
			, lpszArg(parg)
		{}
	};
private:
	CommandRecord*		parseShortOpt(const char* const pOpt, const char* const pArg);
private:
	std::string			m_strCommand;
	std::vector<CommandRecord*>	m_vecCmdRecord;


	typedef std::map<Option, const OptionItem*>	MapOption;
	std::map<std::string, MapOption>	m_mapCmdOption;

	bool			m_bParseSuccess;
	//const int		m_nArgCount;
	//const char * const m_pArgV[];
};

CCommandParser::CCommandParser()
{
}

CCommandParser::~CCommandParser()
{
	m_bParseSuccess = false;
	for (auto it : m_vecCmdRecord)
	{
		delete it;
	}
	m_vecCmdRecord.clear();

	m_strCommand.clear();
	m_mapCmdOption.clear();
}

//void CCommandParser::addOpt(const OptionItem & optItem, const char * lpszCmdName)
//{
//}
//
void CCommandParser::addOpt(const OptionItem * pOptItems, const char * lpszCmdName)
{
	assert(lpszCmdName);
	assert(m_mapCmdOption.find(std::string(lpszCmdName)) == m_mapCmdOption.end());

	//m_mapCmdOption[] = ;
	auto itRet = m_mapCmdOption.insert(std::make_pair(std::string(lpszCmdName), MapOption()));
	int i = 0;
	while (pOptItems[i].nOpt != '\0')
	{
		//itRet.first是m_mapCmdOption的迭代器
		itRet.first->second[pOptItems[i].nOpt] = &pOptItems[i];
		++i;
	}
	//m_mapCmdOption[std::string(lpszCmdName)] = pOptItems;
}

bool CCommandParser::parseCommand(const int argc, const char * const argv[])
{
	if (argv == nullptr)
		return false;
	if (argc < 2)
		return true;

	int argIndex = 1;

	if (argv[1][0] == '-')
	{
		m_strCommand = "";
	}
	else
	{
		m_strCommand = argv[1];
		argIndex++;
	}

	for (; argIndex < argc; ++argIndex)
	{
		auto& pOption = argv[argIndex];
		auto& c1 = pOption[0];
		auto& c2 = pOption[1];

		const char* const arg = (argIndex + 1 < argc) ? argv[argIndex + 1] : nullptr;
		if (c2 == '-')//long mode
		{

		}
		else//short mode
		{
			auto pCommandRecord = parseShortOpt(pOption, arg);
			if (!pCommandRecord)
				return false;
			else
			{
				m_vecCmdRecord.push_back(pCommandRecord);
				if (pCommandRecord->lpszArg != nullptr)
					++argIndex;
			}
		}
	}
	m_bParseSuccess = true;
	return true;
}

const char* CCommandParser::getCommand()const
{
	if (m_bParseSuccess)
		return m_strCommand.c_str();
	else
		return nullptr;
}

int CCommandParser::getOptCount()const
{
	if (!m_bParseSuccess)
		return 0;
	return m_vecCmdRecord.size();
}

ICommandParser::Option CCommandParser::getOpt(const int& nOptIndex)const
{
	if (!m_bParseSuccess)
		return 0;
	if (nOptIndex < 0 || nOptIndex > m_vecCmdRecord.size())
		return 0;
	return m_vecCmdRecord[nOptIndex]->nOpt;
}

const char* CCommandParser::getOptArg(const int& nOptIndex)const
{
	if (!m_bParseSuccess)
		return nullptr;
	if (nOptIndex < 0 || nOptIndex > m_vecCmdRecord.size())
		return nullptr;
	return m_vecCmdRecord[nOptIndex]->lpszArg;
}

void CCommandParser::resetStatu()
{
	m_bParseSuccess = false;
	for (auto it : m_vecCmdRecord)
	{
		delete it;
	}
	m_vecCmdRecord.clear();

	m_strCommand.clear();
	m_mapCmdOption.clear();
}

CCommandParser::CommandRecord * CCommandParser::parseShortOpt(const char * const pOpt, const char * const pArg)
{
	CommandRecord* pCmdRecord = new CommandRecord(pOpt[1]);
	scopeguard::CScopeGuard guardRecord([&pCmdRecord]() {delete pCmdRecord; pCmdRecord = nullptr; });

	auto& mapOption = m_mapCmdOption[m_strCommand];
	auto itOption = mapOption.find(pCmdRecord->nOpt);
	if (itOption == mapOption.end())
	{
		return nullptr;
	}
	else
	{
		auto& pOptionItem = itOption->second;
		if (pOptionItem->has_arg != no)
		{
			if (!pArg || pArg[0] == '-')//期望有参数，但是实际后边没有发现参数
			{
				return nullptr;
			}
			else
			{
				pCmdRecord->lpszArg = pArg;
			}
		}
	}
	guardRecord.Dismiss();
	return pCmdRecord;
}

ICommandParser * CreateCommandParser()
{
	static CCommandParser cmdParser;
	return &cmdParser;
}
