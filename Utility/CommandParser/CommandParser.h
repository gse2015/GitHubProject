#ifndef COMMANDPARSER_H_GSE_2160124
#define COMMANDPARSER_H_GSE_2160124

#define FOR_TEST

class ICommandParser
{
public:
	typedef int	Option;
	enum Has_arg {no, yes/*, maybe*/};

	struct OptionItem
	{
		Option		nOpt;
		Has_arg		has_arg;
		const char*	lpszOptName;
	};

	//struct CommandItem
	//{
	//	const char*	lpszCmdName;
	//	OptionItem*	pOptItems;
	//};

	//virtual void		addOpt(const OptionItem& optItem, const char* lpszCmdName) = 0;
	virtual void		addOpt(const OptionItem* pOptItems, const char* lpszCmdName) = 0;

	virtual bool		parseCommand(const int argc, const char * const argv[]) = 0;

	virtual const char*	getCommand()const = 0;
	virtual int			getOptCount()const = 0;
	virtual Option		getOpt(const int& nOptIndex)const = 0;
	virtual const char*	getOptArg(const int& nOptIndex)const = 0;
#ifdef FOR_TEST
	virtual void		resetStatu() = 0;
#endif
};

ICommandParser* CreateCommandParser();
#endif//COMMANDPARSER_H_GSE_2160124
