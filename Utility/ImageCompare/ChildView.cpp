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

// ChildView.cpp : CChildView ���ʵ��
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



// CChildView ��Ϣ�������

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
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	
	// TODO:  �ڴ˴������Ϣ����������
	if (m_ImgContext_Left && m_ImgContext_Right)
	{
		CRect rcClient;
		GetClientRect(rcClient);

		CRect rcLeft(rcClient.left, rcClient.top, rcClient.Width() / 2 - 3, rcClient.Height());
		CRect rcRight(rcClient.Width() / 2 + 3, rcClient.top, rcClient.Width(), rcClient.Height());

		ImageOperation::showImage(m_ImgContext_Left, dc.m_hDC, rcLeft.left, rcLeft.top, rcLeft.Width(), rcLeft.Height());
		ImageOperation::showImage(m_ImgContext_Right, dc.m_hDC, rcRight.left, rcRight.top, rcRight.Width(), rcRight.Height());
	}
	// ��ҪΪ������Ϣ������ CWnd::OnPaint()
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
	// TODO:  �ڴ���������������
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
	// TODO:  �ڴ������������û����洦��������
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
	// TODO:  �ڴ���������������;
	const wchar_t* wcSupportExt[] = { L"xml" };
	auto uCount = _countof(wcSupportExt);
	m_vecResultXml.clear();
	m_nNextResult = -1;
	traversalAllSupportFiles(m_strResultPath.c_str(), m_vecResultXml, wcSupportExt, uCount, false);
	showNextResult();
}


void CChildView::OnUpdateBtnCompare(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
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
