
// secure-email-testsDlg.h : header file
//

#pragma once
#include "cernsecurity.h"

// CsecureemailtestsDlg dialog
class CsecureemailtestsDlg : public CDialogEx
{
// Construction
public:
   CsecureemailtestsDlg(CWnd* pParent = nullptr);	// standard constructor

   enum { IDD = IDD_SECUREEMAILTESTS_DIALOG };

   protected:
   virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
   CWnd* m_pParent;
   HICON m_hIcon;
   bool m_LoggedIn;
   CString m_sUserName;
   CString m_sDomain;
   CrmHandle m_hApp;
   CCernSecurity	m_ctrlCernSecurity;
   HANDLE m_LoginEvent;

	// Generated message map functions
   virtual BOOL OnInitDialog();
   afx_msg void OnPaint();
   afx_msg HCURSOR OnQueryDragIcon();
   DECLARE_MESSAGE_MAP()
   DECLARE_DYNCREATE(CsecureemailtestsDlg)
   afx_msg LRESULT OnLogin(WPARAM, LPARAM);
   afx_msg LRESULT OnLoginDone(WPARAM, LPARAM);

   DECLARE_EVENTSINK_MAP()
   afx_msg void OnLoginResultCernsecurity(short Status);
   afx_msg void OnShutDownCernsecurity();

   virtual void PostNcDestroy();
};
