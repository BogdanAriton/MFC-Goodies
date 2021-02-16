// CMainDlg.cpp : implementation file
//

#include "pch.h"
#include "secure-email-tests.h"
#include "CMainDlg.h"
#include "afxdialogex.h"
#include "secure-email-testsDlg.h"


// CMainDlg dialog

IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN, pParent)
{

}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
END_MESSAGE_MAP()


// CMainDlg message handlers


BOOL CMainDlg::OnInitDialog()
{
   CDialogEx::OnInitDialog();

   // TODO:  Add extra initialization here
   m_testDlg = std::make_unique<CsecureemailtestsDlg>(this);
   INT_PTR nResponse = m_testDlg->DoModal();
   if (nResponse == IDOK)
   {
      TRACE(traceAppMsg, 0, "Login worked!.\n");
   }
   else
   {
      if (nResponse == IDCANCEL)
      {
         TRACE(traceAppMsg, 0, "Login Failed!.\n");
      }
   }

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
