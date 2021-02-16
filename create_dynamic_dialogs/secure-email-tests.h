
// secure-email-tests.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// TestApp:
// See secure-email-tests.cpp for the implementation of this class
//
class CMainDlg;
class TestApp : public CWinApp
{
public:
   std::unique_ptr<CMainDlg> m_mainDlg;
   std::unique_ptr<CWnd> m_myWin;
	TestApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
   DECLARE_DYNCREATE(TestApp)
	DECLARE_MESSAGE_MAP()
};

extern TestApp theApp;
