#pragma once


// CFileSelectDlg �Ի���

class CFileSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileSelectDlg)

public:
	CFileSelectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileSelectDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
