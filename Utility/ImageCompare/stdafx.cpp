// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// ImageCompare.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

#include <io.h>
#include <algorithm>
#include <stdlib.h>


std::string wcs2mbs(const std::wstring& strSrc)
{
	auto nNeed = ::WideCharToMultiByte(CP_UTF8, 0, strSrc.c_str(), strSrc.size(), nullptr, 0, nullptr, nullptr);
	char* ch = new char[nNeed + 1];
	memset(ch, 0, nNeed + 1);
	nNeed = ::WideCharToMultiByte(CP_UTF8, 0, strSrc.c_str(), strSrc.size(), ch, nNeed, nullptr, nullptr);
	std::string str(ch);
	delete []ch;
	return str;
}

std::wstring mbs2wcs(const std::string& strSrc)
{
	auto nNeed = ::MultiByteToWideChar(CP_UTF8, 0, strSrc.c_str(), strSrc.size(), nullptr, 0);
	wchar_t* ch = new wchar_t[nNeed+1];
	memset(ch, 0, (nNeed+1)*sizeof(wchar_t));
	nNeed = ::MultiByteToWideChar(CP_UTF8, 0, strSrc.c_str(), strSrc.size(), ch, nNeed);
	std::wstring str(ch);
	delete[]ch;
	return str;
}

bool isSupportedExt(const wchar_t**& wcSupportExt, const unsigned int& uCountExt, const wchar_t* pFilePath);

bool traversalAllSupportFiles(const wchar_t* pDir, std::vector<std::wstring>& filesVec, const wchar_t** wcSupportExt, const unsigned int& uCountExt
	, bool bTraversalSub /*= false*/, const std::wstring strParentDir /*= L""*/)
{
	_wfinddata_t fd;
	std::wstring wsFile = pDir;
	wsFile += L"\\*";
	// 	wsFile += wcSupportExt[0];
	intptr_t nHandler = _wfindfirst(wsFile.c_str(), &fd);
	if (nHandler == -1)
		return false;
	do
	{
		if ((fd.attrib & _A_SUBDIR) && bTraversalSub)
		{
			if (wcscmp(fd.name, L".") != 0 && wcscmp(fd.name, L"..") != 0)
			{
				std::wstring strDir = pDir;
				strDir += L"\\";
				strDir += fd.name;
				traversalAllSupportFiles(strDir.c_str(), filesVec, wcSupportExt, uCountExt, bTraversalSub, fd.name);
			}
		}
		else
		{
			if (isSupportedExt(wcSupportExt, uCountExt, fd.name))
			{
				std::wstring strFile = strParentDir;
				if (strParentDir.size() > 0)
					strFile += L'\\';
				strFile += fd.name;
				filesVec.push_back(strFile);
			}
		}
	} while (_wfindnext(nHandler, &fd) != -1);

	_findclose(nHandler);
	errno_t e = errno;
	return true;
}

bool isSupportedExt(const wchar_t**& wcSupportExt, const unsigned int& uCountExt, const wchar_t* pFilePath)
{
	if (!pFilePath)
		return false;
	wchar_t wcExt[50] = { 0 };
	for (int j = wcslen(pFilePath) - 1, nIndex = 0; j >= 0; --j)
	{
		if (pFilePath[j] == L'.')
		{
			wcscpy_s(wcExt, pFilePath + j + 1);
			break;
		}
	}

	for (unsigned int i = 0; i < uCountExt; ++i)
	{
		if (_wcsicmp(wcExt, wcSupportExt[i]) == 0)
			return true;
	}
	return false;
}
