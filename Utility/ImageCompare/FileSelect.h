#pragma once


// CFileSelectDlg 对话框

class CFileSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileSelectDlg)

public:
	CFileSelectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileSelectDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString		m_strOldVersion;
	CString		m_strNewVersion;
	CString		m_strResult;
public:
	const CString&	GetOldVersion() { return m_strOldVersion; }
	const CString&	GetNewVersion() { return m_strNewVersion; }
	const CString&	GetResult() { return m_strResult; }
public:
	afx_msg void OnBnClickedOk();
};
