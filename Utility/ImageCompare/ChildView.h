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

// ChildView.h : CChildView 类的接口
//


#pragma once

#include "ImageOperation.h"

// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 特性
public:

// 操作
public:

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
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

