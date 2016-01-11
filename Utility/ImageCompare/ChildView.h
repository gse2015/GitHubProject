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

// ChildView.h : CChildView ��Ľӿ�
//


#pragma once

#include "ImageOperation.h"

// CChildView ����

class CChildView : public CWnd
{
// ����
public:
	CChildView();

// ����
public:

// ����
public:

// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBtnOpen();
	afx_msg void OnUpdateBtnOpen(CCmdUI *pCmdUI);
	afx_msg void OnBtnCompare();
	afx_msg void OnUpdateBtnCompare(CCmdUI *pCmdUI);
	afx_msg void OnBtnShowResult();

private:
	ImageContext*		m_ImgContext_Left;
	ImageContext*		m_ImgContext_Right;

	std::wstring		m_strOldVersionPath;
	std::wstring		m_strNewVersionPath;
	std::wstring		m_strResultPath;

	std::vector<std::wstring>	m_vecResultXml;
	int					m_nNextResult;
private:
	CompareResult*	loadResult(const std::wstring& strResultXml);
	bool			showResult(const CompareResult* cmpResult);

	bool			showNextResult();
	bool			showPrevResult();
public:
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

