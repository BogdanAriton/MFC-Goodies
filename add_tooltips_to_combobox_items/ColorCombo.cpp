// ColorCombo.cpp : implementation file
//

#include "stdafx.h"
#include "ColorCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CColorCombo::CColorCombo(COleControl* pCtrl)
{
   EnableActiveAccessibility();

   m_pCtrl = pCtrl;
   m_hWndToolTip = NULL;
   m_background = 0xFFFFFF;
   m_brush.CreateSolidBrush(m_background);
   m_pCtrlHasFocus = false;
   m_bToolActive = false;
   m_toolInfo = { 0 };
}

CColorCombo::~CColorCombo()
{
   m_hWndToolTip = NULL;
   if (NULL != m_ListBox.GetSafeHwnd())
      m_ListBox.UnsubclassWindow();
}

BEGIN_MESSAGE_MAP(CColorCombo, CComboBox)
   ON_WM_CTLCOLOR()
   ON_WM_CTLCOLOR_REFLECT()
   ON_WM_SETFOCUS()
   ON_WM_KILLFOCUS()
   ON_WM_LBUTTONDOWN()
   ON_WM_MOUSEMOVE()
   ON_WM_TIMER()
END_MESSAGE_MAP()

void CColorCombo::SetBackColor(COLORREF color)
{
   if (color != m_background)
   {
      m_background = color;
      m_brush.DeleteObject();
      m_brush.CreateSolidBrush(m_background);
      if (GetSafeHwnd())
      {
         RedrawWindow();
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
// CColorCombo message handlers

HBRUSH CColorCombo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
   switch (nCtlColor)
   {
   case CTLCOLOR_LISTBOX:
      pDC->SetBkColor(m_background);
      return m_brush;

   default:
	   return CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
   }
}

HBRUSH CColorCombo::CtlColor(CDC* pDC, UINT nCtlColor) 
{
   switch (nCtlColor)
   {
   case CTLCOLOR_EDIT:
      pDC->SetBkColor(m_background);
      return m_brush;

   default:
	   return NULL;
   }
}

void CColorCombo::OnSetFocus(CWnd* pOldWnd) 
{
   // If DBPrint control does not have focus, set focus so it is aware that the combo box has focus.
   if (!m_pCtrlHasFocus)
   {
      m_pCtrl->GetSafeHwnd();
      m_pCtrl->SetFocus();
   }
   else
   {
      CComboBox::OnSetFocus(pOldWnd);
   }
}

void CColorCombo::OnKillFocus(CWnd* pNewWnd) 
{
   // Combo box lost focus
   CComboBox::OnKillFocus(pNewWnd);
   m_pCtrlHasFocus = false;
}

void CColorCombo::PCtrlGotFocus()
{
   // Set memeber variable to denote DBPrint control got focus.
   m_pCtrlHasFocus = true;
}

HRESULT CColorCombo::get_accName(VARIANT varChild, BSTR *pszName)
{
   if (varChild.lVal != CHILDID_SELF)
   {
      return CComboBox::get_accName(varChild, pszName);
   }
   
   if (varChild.vt != VT_I4)
   {
      *pszName = NULL;
      return E_INVALIDARG;
   }
   
   CComPtr<IAccessible> pac;
   HRESULT hr = AccessibleObjectFromWindow(m_pCtrl->GetSafeHwnd(), OBJID_WINDOW, IID_IAccessible, (void**)&pac);
   if (FAILED(hr))
   {
      return hr;
   }

   VARIANT childid_self;
   childid_self.vt = VT_I4;
   childid_self.lVal = CHILDID_SELF;
   return pac->get_accName(childid_self, pszName);
}


void CColorCombo::OnLButtonDown(UINT nFlags, CPoint point)
{
   // post this custom message that will be capture by DBPrint control to be able to open the addressbook on left mouse button pressed
   ::PostMessage(m_pCtrl->GetSafeHwnd(), WM_COMBO_MOUSEDOWN, 0, 0);
   CComboBox::OnLButtonDown(nFlags, point);
}

void CColorCombo::OnMouseMove(UINT nFlags, CPoint point)
{
   CComboBox::OnMouseMove(nFlags, point);

   if (m_hWndToolTip == NULL)
      return;

   CRect rectClient;
   GetClientRect(&rectClient);
   const int nComboButtonWidth = GetSystemMetrics(SM_CXHTHUMB) + 2;
   rectClient.right = rectClient.right - nComboButtonWidth; // calculate the width of the combo

   if (rectClient.PtInRect(point) && !GetDroppedState()) // we show the tooltip if the mouse is over the combo and if the dropdown is not visible
   {
      ClientToScreen(&rectClient);
      CString sText;
      GetWindowText(sText);

      HDC hDC = ::GetDC(m_hWnd);
      ASSERT(hDC);
      CFont* pFont = GetFont();
      HFONT hOldFont = (HFONT)::SelectObject(hDC, (HFONT)*pFont);
      SIZE size;
      ::GetTextExtentPoint32(hDC, sText, sText.GetLength(), &size); // calculate the size of the text within the combo
      ::SelectObject(hDC, hOldFont);
      ::ReleaseDC(m_hWnd, hDC);

      if (!m_bToolActive && size.cx > rectClient.Width() - 6)
      {
         // formatting the text to show on multiple lines
         sText.Format("%s", CColorComboList::ReturnMultilineToolTip(sText).c_str());
         m_toolInfo.lpszText = (LPTSTR)(LPCTSTR)sText;

         // if the tool is not active and the text size is larger than the list width we will show the tooltip
         rectClient.left += 1;
         rectClient.top += 3;
         ::SendMessage(m_hWndToolTip, TTM_UPDATETIPTEXT, (WPARAM)0, (LPARAM)&m_toolInfo);
         ::SendMessage(m_hWndToolTip, TTM_TRACKPOSITION, (WPARAM)0, (LPARAM)MAKELONG(rectClient.left, rectClient.top - nComboButtonWidth - 1));
         ::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)(LPTOOLINFO)&m_toolInfo);
         m_bToolActive = true;
         SetTimer(1, 80, NULL);
      }
      else
      {
         // text fits inside client rect no need to the tooltip
         SetTimer(1, 80, NULL);
      }
   }
   else	// not inside client rect
   {
      ::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)(LPTOOLINFO)&m_toolInfo);
      m_bToolActive = false;
      SetTimer(1, 80, NULL);
   } 
}

bool CColorCombo::AddToolTip(int comboID, HWND hwndCombo, HWND hDlg, PTSTR pszText)
{
   if (!comboID || !hDlg || !pszText)
   {
      return false;
   }
   // Create the tooltip window
   m_hWndToolTip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
      TTS_NOPREFIX | TTS_ALWAYSTIP,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      hDlg, NULL,
      NULL, NULL);


   if (!hwndCombo || !m_hWndToolTip)
   {
      return false;
   }

   // initialize toolinfo struct
   memset(&m_toolInfo, 0, sizeof(m_toolInfo));
   // Associate the tooltip with the tool.
   m_toolInfo.cbSize = sizeof(TTTOOLINFOA_V2_SIZE);
   m_toolInfo.hwnd = m_hWnd;
   m_toolInfo.uFlags = TTF_TRACK | TTF_TRANSPARENT;
   m_toolInfo.lpszText = pszText;
   ::SendMessage(m_hWndToolTip, TTM_SETMAXTIPWIDTH, (WPARAM)0, (LPARAM)SHRT_MAX);
   ::SendMessage(m_hWndToolTip, TTM_ADDTOOL, 0, (LPARAM)&m_toolInfo);

   return true;
}

BOOL CColorCombo::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
   if (!CComboBox::Create(dwStyle, rect, pParentWnd, nID))
      return FALSE;

   if (!AddToolTip(IDC_PRINT_BUTTON, this->GetSafeHwnd(), m_pCtrl->GetSafeHwnd(), ""))
      return FALSE;

   m_comboInfo = { sizeof(COMBOBOXINFO) };
   ::GetComboBoxInfo(this->GetSafeHwnd(), &m_comboInfo); // get the combo info structure that hold the handle to the listbox

   if (NULL != m_comboInfo.hwndList)
   {
      // creates the ColorComboList as part of the PreSubClass event 
      // and link it with the ColorCombo listbox to be able to capture mouse move events
      if (!m_ListBox.SubclassWindow(m_comboInfo.hwndList)) 
         TRACE("Unable to subclass the listBox"); // not much else we can do here (the tooltip for the list box will not be loaded)
   }

   return TRUE;
}


void CColorCombo::OnTimer(UINT_PTR nIDEvent)
{
   CComboBox::OnTimer(nIDEvent);
   
   if (m_hWndToolTip == NULL)
      return;

   CPoint point;
   ::GetCursorPos(&point);
   ScreenToClient(&point);

   CRect rectClient;
   GetClientRect(&rectClient);
   int nComboButtonWidth = GetSystemMetrics(SM_CXHTHUMB) + 2;
   rectClient.right = rectClient.right - nComboButtonWidth;

   if (!rectClient.PtInRect(point))
   {
      KillTimer(nIDEvent);
      ::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)(LPTOOLINFO)&m_toolInfo);
      m_bToolActive = FALSE;
   }
}
