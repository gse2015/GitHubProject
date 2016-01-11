// FileSelect.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageCompare.h"
#include "FileSelect.h"
#include "afxdialogex.h"


// CFileSelectDlg 对话框

IMPLEMENT_DYNAMIC(CFileSelectDlg, CDialogEx)

CFileSelectDlg::CFileSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_strOldVersion(_T(""))
{

}

CFileSelectDlg::~CFileSelectDlg()
{
}

void CFileSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OLDVERSION, m_strOldVersion);
	DDX_Text(pDX, IDC_EDIT_NEWVERSION, m_strNewVersion);
	DDX_Text(pDX, IDC_EDIT_RESULT, m_strResult);
}


BEGIN_MESSAGE_MAP(CFileSelectDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFileSelectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFileSelectDlg 消息处理程序


void CFileSelectDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
