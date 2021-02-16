#pragma once


// CMainDlg dialog
class CsecureemailtestsDlg;

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CMainDlg();

	enum { IDD = IDD_MAIN };

protected:
   std::unique_ptr<CsecureemailtestsDlg> m_testDlg;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();
};
