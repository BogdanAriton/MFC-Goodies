
// secure-email-testsDlg.cpp : implementation file
//

#include "pch.h"
#include <chrono>
#include <thread>
#include "framework.h"
#include "secure-email-tests.h"
#include "secure-email-testsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsecureemailtestsDlg dialog

IMPLEMENT_DYNCREATE(CsecureemailtestsDlg, CDialogEx)

CsecureemailtestsDlg::CsecureemailtestsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SECUREEMAILTESTS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
   m_LoggedIn = false;
   m_LoginEvent = INVALID_HANDLE_VALUE;
   m_pParent = pParent;
}

void CsecureemailtestsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_EVENTSINK_MAP(CsecureemailtestsDlg, CDialogEx)
   //{{AFX_EVENTSINK_MAP(CMainFrame)
   ON_EVENT(CsecureemailtestsDlg, IDC_CERNSECURITY, 1 /* LoginResult */, OnLoginResultCernsecurity, VTS_I2)
   ON_EVENT(CsecureemailtestsDlg, IDC_CERNSECURITY, 2 /* ShutDown */, OnShutDownCernsecurity, VTS_NONE)
   //}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BEGIN_MESSAGE_MAP(CsecureemailtestsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
   ON_MESSAGE(WM_LOGIN, OnLogin)
   ON_MESSAGE(WM_LOGINDONE, OnLoginDone)
   ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()


// CsecureemailtestsDlg message handlers

BOOL CsecureemailtestsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
   CoInitialize(NULL);
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
   PostMessage(WM_LOGIN);
   CoUninitialize();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CsecureemailtestsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CsecureemailtestsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CsecureemailtestsDlg::OnLogin(WPARAM wParam, LPARAM lParam)
{
   ShowWindow(SW_HIDE);
   UpdateWindow();
   HANDLE handle = reinterpret_cast<HANDLE>(lParam);
   if (lParam && (handle != INVALID_HANDLE_VALUE))
   {
      if ((m_LoginEvent != INVALID_HANDLE_VALUE) || m_LoggedIn)
      {
         SetEvent(handle);
         return FALSE;
      }

      m_LoginEvent = handle;
   }

   // Lets create the security context here :D
   CRect securityRect(0, 0, 0, 0);
   if (!m_ctrlCernSecurity.Create("CernSecurity", WS_CHILD | WS_BORDER | WS_HSCROLL, securityRect, this, IDC_CERNSECURITY))
   {
      TRACE0("Failed to create security dialog");
      PostMessage(WM_CLOSE);
   }
   //short result = m_ctrlCernSecurity.Login();

   //short result = m_ctrlCernSecurity.SilentLogin("ba041626","solgm","");
   short result = m_ctrlCernSecurity.SilentLogin("ba041626", "solgm", "08114219Hasdeu-8");
   if (result != CERNSEC_GNRL_SUCCESS)			//login failed
   {
      PostMessage(WM_CLOSE);					//close the app
   }
   return 0;
}

LRESULT CsecureemailtestsDlg::OnLoginDone(WPARAM wParam, LPARAM)
{
   if (wParam == CERNSEC_GNRL_SUCCESS)
   {
      m_LoggedIn = true;
   }
   else
      m_LoggedIn = false;

   if (m_LoginEvent == INVALID_HANDLE_VALUE)
   {
      if (m_LoggedIn)
      {
         CrmStatus crmStat = CrmBeginApp(5000, &m_hApp);
         switch (crmStat)
         {
         case eCrmSecContextErr:
            AfxMessageBox(_T("CrmBeginApp failed!  Missing security context! Login was probably not performed. "));
            exit(-1);
            break;
         case eCrmAppAuthAppNotFound:
            AfxMessageBox(_T("CrmBeginApp failed!  Application could not be found or the user doesn't have access. "));
            exit(-1);
            break;
         case eCrmOk:
            break;
         default:
            char buf[10];
            _itoa_s(crmStat, buf, 10, 10);
            AfxMessageBox(_T("CrmBeginApp failed!  status= ") + (CString)buf);
            exit(-1);
         }
         ShowWindow(SW_HIDE);
         UpdateWindow();
         PostMessage(WM_CLOSE);
      }
      else
         PostMessage(WM_CLOSE);
   }
   else
   {
      SetEvent(m_LoginEvent);
      m_LoginEvent = INVALID_HANDLE_VALUE;
   }
   return TRUE;
}

void CsecureemailtestsDlg::OnLoginResultCernsecurity(short Status)
{
   if (Status == CERNSEC_GNRL_SUCCESS)
   {
      m_sUserName = m_ctrlCernSecurity.GetUsername();
      m_sDomain = m_ctrlCernSecurity.GetDomain();
   }

   PostMessage(WM_LOGINDONE, Status);
}

void CsecureemailtestsDlg::OnShutDownCernsecurity()
{
   PostMessage(WM_CLOSE);
}


void CsecureemailtestsDlg::PostNcDestroy()
{
   CDialogEx::PostNcDestroy();
}
