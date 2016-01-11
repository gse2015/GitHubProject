#pragma once

#include <string>
class CImgCompare
{
public:
	virtual ~CImgCompare();
	static CImgCompare*	getImgCmp();
	bool				compare(const wchar_t* wcOldVersionPath, const wchar_t* wcNewVersionPath, const wchar_t* wcResultPath);
protected:
	CImgCompare();
private:

	std::wstring	m_strOldVersionPath;
	std::wstring	m_strNewVersionPath;
};

