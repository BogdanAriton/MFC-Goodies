
// secure-email-tests.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "secure-email-tests.h"
#include "CMainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// TestApp

IMPLEMENT_DYNCREATE(TestApp, CWinApp)

BEGIN_MESSAGE_MAP(TestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// TestApp construction

TestApp::TestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only TestApp object

TestApp theApp;


// TestApp initialization

BOOL TestApp::InitInstance()
{
   // InitCommonControlsEx() is required on Windows XP if an application
   // manifest specifies use of ComCtl32.dll version 6 or later to enable
   // visual styles.  Otherwise, any window creation will fail.
   INITCOMMONCONTROLSEX InitCtrls;
   InitCtrls.dwSize = sizeof(InitCtrls);
   // Set this to include all the common control classes you want to use
   // in your application.
   InitCtrls.dwICC = ICC_WIN95_CLASSES;
   InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

   AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
   /*m_mainDlg = std::make_unique<CsecureemailtestsDlg>();
   m_mainDlg->Create(CsecureemailtestsDlg::IDD);
   m_pMainWnd = m_mainDlg.get();*/
   try
   {
      m_myWin = std::make_unique<CMainDlg>(CWnd::GetDesktopWindow());
      if (m_myWin != NULL && m_myWin->IsKindOf(RUNTIME_CLASS(CMainDlg)))
      {
         m_mainDlg = std::make_unique<CMainDlg>(m_myWin.get());
         m_mainDlg->AssertValid();
         m_mainDlg->Create(CMainDlg::IDD, CWnd::GetDesktopWindow());
         m_pMainWnd = m_mainDlg.get();
      }
   }
   catch (...)
   {
      // capture anything
   }
	// Delete the shell manager created above.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

