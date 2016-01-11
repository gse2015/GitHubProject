#include "StdAfx.h"
#include "ImgCompare.h"
#include "ImageOperation.h"


CImgCompare::CImgCompare()
{
}


CImgCompare::~CImgCompare()
{
}

CImgCompare* CImgCompare::getImgCmp()
{
	static CImgCompare imgCmp;
	return &imgCmp;
}

bool CImgCompare::compare(const wchar_t* wcOldVersionPath, const wchar_t* wcNewVersionPath, const wchar_t* wcResultPath)
{
	const wchar_t* wcSupportExt[] = { L"bmp" };
	auto uCount = _countof(wcSupportExt);
	std::wstring strOldVersion(wcOldVersionPath);
	std::wstring strNewVersion(wcNewVersionPath);

	m_strOldVersionPath = wcOldVersionPath;
	m_strNewVersionPath = wcNewVersionPath;

	std::vector<std::wstring> OldVersionFiles;
	traversalAllSupportFiles(strOldVersion.c_str(), OldVersionFiles, wcSupportExt, uCount, false);
	auto itOldFile = OldVersionFiles.begin();

	SHCreateDirectory(nullptr, wcResultPath);

	while (itOldFile != OldVersionFiles.end())
	{
		try
		{
			auto pImgBase = ImageOperation::openImage(strOldVersion + L"\\" + *itOldFile);
			auto pImgNew = ImageOperation::openImage(strNewVersion + L"\\" + *itOldFile);
			CompareResult* pResult = ImageOperation::compare(pImgBase, pImgNew);
	
			ImageOperation::writeResultToXml(pResult, wcs2mbs(std::wstring(wcResultPath) + L"\\" + *itOldFile + L".xml"));
			ImageOperation::closeImage(pImgBase);
			ImageOperation::closeImage(pImgNew);
			delete pResult;
			pResult = nullptr;
		}
		catch (CMemoryException* e)
		{
		}
		catch (CFileException* e)
		{
		}
		catch (CException* e)
		{
		}
		catch (...)
		{
		}
		itOldFile++;
	}
	return true;
}
