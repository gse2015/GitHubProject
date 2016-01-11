// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "ImageCompare.h"
#include "ImgCompare.h"
#include "ChildView.h"
#include <vector>

#include "FileSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
	: m_ImgContext_Left(nullptr)
	, m_ImgContext_Right(nullptr)
	, m_nNextResult(-1)
{
}

CChildView::~CChildView()
{
	if (m_ImgContext_Left)
		ImageOperation::closeImage(m_ImgContext_Left);
	if (m_ImgContext_Right)
		ImageOperation::closeImage(m_ImgContext_Right);
	m_nNextResult = -1;
	m_vecResultXml.clear();
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_BTN_OPEN, &CChildView::OnBtnOpen)
	ON_UPDATE_COMMAND_UI(ID_BTN_OPEN, &CChildView::OnUpdateBtnOpen)
	ON_COMMAND(ID_BTN_COMPARE, &CChildView::OnBtnCompare)
	ON_UPDATE_COMMAND_UI(ID_BTN_COMPARE, &CChildView::OnUpdateBtnCompare)
	ON_COMMAND(ID_BTN_SHOWRESULT, &CChildView::OnBtnShowResult)
	ON_WM_KEYUP()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	// TODO:  在此处添加消息处理程序代码
	if (m_ImgContext_Left && m_ImgContext_Right)
	{
		CRect rcClient;
		GetClientRect(rcClient);

		CRect rcLeft(rcClient.left, rcClient.top, rcClient.Width() / 2 - 3, rcClient.Height());
		CRect rcRight(rcClient.Width() / 2 + 3, rcClient.top, rcClient.Width(), rcClient.Height());

		ImageOperation::showImage(m_ImgContext_Left, dc.m_hDC, rcLeft.left, rcLeft.top, rcLeft.Width(), rcLeft.Height());
		ImageOperation::showImage(m_ImgContext_Right, dc.m_hDC, rcRight.left, rcRight.top, rcRight.Width(), rcRight.Height());
	}
	// 不要为绘制消息而调用 CWnd::OnPaint()
}



void CChildView::OnBtnOpen()
{
	m_strOldVersionPath = L"C:\\Users\\GSE\\Desktop\\Old";
	m_strNewVersionPath = L"C:\\Users\\GSE\\Desktop\\New";
	m_strResultPath		= L"C:\\Users\\GSE\\Desktop\\Result";
	CFileSelectDlg dlg;
	dlg.m_strNewVersion	= m_strNewVersionPath.c_str();
	dlg.m_strOldVersion	= m_strOldVersionPath.c_str();
	dlg.m_strResult		= m_strResultPath.c_str();
	if (dlg.DoModal() == IDOK)
	{
		m_strOldVersionPath = dlg.m_strOldVersion;
		m_strNewVersionPath = dlg.m_strNewVersion;
		m_strResultPath = dlg.m_strResult;
		CImgCompare* pImgCmp = CImgCompare::getImgCmp();
		pImgCmp->compare(m_strOldVersionPath.c_str(), m_strNewVersionPath.c_str(), m_strResultPath.c_str());
	}
	// TODO:  在此添加命令处理程序代码
	//CString strFilter = L"Image Files|*.bmp;*.png;*.jpg|";
	//CFileDialog fd(TRUE, L"*.bmp", nullptr, 6, strFilter, this);
	//if (fd.DoModal() == IDOK)
	//{
	//	if (m_ImgContext_Left)
	//		ImageOperation::closeImage(m_ImgContext_Left);
	//	m_ImgContext_Left = ImageOperation::openImage(std::wstring(fd.GetPathName()));
	//	InvalidateRect(nullptr);
	//}
}


void CChildView::OnUpdateBtnOpen(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(TRUE);
}

void CChildView::OnBtnShowResult()
{
	const wchar_t* wcSupportExt[] = { L"xml" };
	auto uCount = _countof(wcSupportExt);
	m_vecResultXml.clear();
	m_nNextResult = -1;
	traversalAllSupportFiles(m_strResultPath.c_str(), m_vecResultXml, wcSupportExt, uCount, false);
	showNextResult();
// 	auto itResultFile = m_vecResultXml.begin();

// 	CFileSelectDlg dlg;
// 	dlg.m_strNewVersion = L"C:\\Users\\GSE\\Desktop\\New";
// 	dlg.m_strOldVersion = L"C:\\Users\\GSE\\Desktop\\Old";
// 	dlg.m_strResult = L"C:\\Users\\GSE\\Desktop\\Result";
// 	if (dlg.DoModal() == IDOK)
// 	{
// 		const auto& strOldVersion = dlg.m_strOldVersion;
// 		const auto& strNewVersion = dlg.m_strNewVersion;
// 		const auto& strResult = dlg.m_strResult;
// 		CImgCompare* pImgCmp = CImgCompare::getImgCmp();
// 		pImgCmp->showCmpResult(strOldVersion, strNewVersion, strResult);
// 	}
}

CompareResult* CChildView::loadResult(const std::wstring& strResultXml)
{
	return ImageOperation::loadResultFormXml(wcs2mbs(strResultXml));
}

bool CChildView::showResult(const CompareResult* cmpResult)
{
	assert(cmpResult);
	if (!cmpResult)
		return false;
	if (m_ImgContext_Left)
	{
		ImageOperation::closeImage(m_ImgContext_Left);
		m_ImgContext_Left = nullptr;
	}
	if (m_ImgContext_Right)
	{
		ImageOperation::closeImage(m_ImgContext_Right);
		m_ImgContext_Right = nullptr;
	}

	m_ImgContext_Left = ImageOperation::openImage(mbs2wcs(cmpResult->getFirstImgFilePath()));
	m_ImgContext_Right = ImageOperation::openImage(mbs2wcs(cmpResult->getSecondImgFilePath()));
	assert(m_ImgContext_Left && m_ImgContext_Right);
	if (!m_ImgContext_Left || !m_ImgContext_Right)
		return false;
	ImageOperation::markDifference(m_ImgContext_Right, cmpResult);
	InvalidateRect(nullptr, FALSE);
	return true;
}

bool CChildView::showNextResult()
{
	++m_nNextResult;
	if (m_nNextResult < 0 || m_nNextResult >= m_vecResultXml.size())
	{
		m_nNextResult = -1;
		return false;
	}
	auto resultXml = m_vecResultXml[m_nNextResult];
	auto pResult = loadResult(m_strResultPath + L"\\" + resultXml);
	auto bRet = showResult(pResult);
	delete pResult;
	return bRet;
}

bool CChildView::showPrevResult()
{
	--m_nNextResult;
	if (m_nNextResult < 0 || m_nNextResult >= m_vecResultXml.size())
	{
		m_nNextResult = -1;
		return false;
	}
	auto resultXml = m_vecResultXml[m_nNextResult];
	auto pResult = loadResult(m_strResultPath + L"\\" + resultXml);
	auto bRet = showResult(pResult);
	delete pResult;
	return bRet;
}

void CChildView::OnBtnCompare()
{
	// TODO:  在此添加命令处理程序代码;
	const wchar_t* wcSupportExt[] = { L"xml" };
	auto uCount = _countof(wcSupportExt);
	m_vecResultXml.clear();
	m_nNextResult = -1;
	traversalAllSupportFiles(m_strResultPath.c_str(), m_vecResultXml, wcSupportExt, uCount, false);
	showNextResult();
}


void CChildView::OnUpdateBtnCompare(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(TRUE);
}


void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_RIGHT || nChar == 'n' || nChar == 'N')
	{
		showNextResult();
	}
	else if (nChar == VK_LEFT || nChar == 'p' || nChar == 'P')
	{
		showPrevResult();
	}
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}
