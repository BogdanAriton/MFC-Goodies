#include "StdAfx.h"
#include "ColorComboList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CColorComboList, CListBox)
   ON_WM_MOUSEMOVE()
   ON_WM_TIMER()
END_MESSAGE_MAP()


CColorComboList::CColorComboList()
{
   m_bToolActive = false;
   m_hWndToolTip = NULL;
   m_toolInfo = { 0 };
}

const std::string CColorComboList::ReturnMultilineToolTip(const CString& toolTipText)
{
   std::string text(toolTipText);
   std::string returnString = std::string();
   auto getSplit = [=](std::string emailStr, int method) {
      if (emailStr.find(strMethod[method]) != std::string::npos)
         return SplitByEmail(emailStr);
      else
         return std::string();
   };

   std::vector<std::string> emailsByMethod = {};
   std::string emailList;
   std::stringstream textToStream(text);

   while (std::getline(textToStream, emailList, ';'))
   {
      emailsByMethod.push_back(emailList);
   }

   for (auto item : emailsByMethod)
   {
      for (int method = Method::TO; method != Method::END; method++)
      {
         std::string emailSplit = getSplit(item, method);
         if (!emailSplit.empty())
         {
            if (item == emailsByMethod.back()) // we don't need to add a new line at the last item in array
            {
               returnString += emailSplit;
            }
            else
            {
               returnString += emailSplit + "\n";
            }
         }
      }
   }
   return returnString;
}

const std::string CColorComboList::SplitByEmail(const std::string& emailList)
{
   std::stringstream emailStream(emailList);
   std::string email;
   std::string resultStream = "";
   int emailNo = 1;
   while (std::getline(emailStream, email, ','))
   {
      if (emailNo != m_iNoEmailsPerLine)
      {
         resultStream += email;
         resultStream += ",";
      }
      else
      {
         resultStream += "\n";
         resultStream += email;
         resultStream += ",";
         emailNo = 1; // reset the email counter
      }
      emailNo++;
   }

   return resultStream.substr(0, resultStream.length() - 1);
}

void CColorComboList::OnMouseMove(UINT nFlags, CPoint point)
{
   CListBox::OnMouseMove(nFlags, point);

   if (m_hWndToolTip == NULL) // in case we don't have a handle to the tooltip window we simply exit
      return;

   CRect rectClient;
   GetClientRect(&rectClient);

   if (rectClient.PtInRect(point)) // if the mouse position is inside the listbox
   {
      CPoint pointScreen;
      ::GetCursorPos(&pointScreen);
      BOOL bOutside = FALSE;
      int nItem = ItemFromPoint(point, bOutside);  // calculate listbox item number (if any)

      if (!bOutside && nItem >= 0)
      {
         m_bToolActive = false;

         CString sText;
         GetText(nItem, sText); // we get the item text based on it's index in the combo item list

         CRect rect;
         GetItemRect(nItem, &rect);
         ClientToScreen(&rect);

         HDC hDC = ::GetDC(m_hWnd);
         ASSERT(hDC);
         CFont* pFont = GetFont();
         HFONT hOldFont = (HFONT)::SelectObject(hDC, (HFONT)*pFont);
         SIZE size;
         ::GetTextExtentPoint32(hDC, sText, sText.GetLength(), &size); // determine the lenght of the text
         ::SelectObject(hDC, hOldFont);
         ::ReleaseDC(m_hWnd, hDC);

         if (!m_bToolActive && size.cx > rect.Width() - 3)
         {
            // formatting the text to show on multiple lines
            sText.Format("%s", CColorComboList::ReturnMultilineToolTip(sText).c_str());
            m_toolInfo.lpszText = (LPTSTR)(LPCTSTR)sText;

            // if the tool is not active and the text size is larger than the list width we will show the tooltip
            ::SendMessage(m_hWndToolTip, TTM_UPDATETIPTEXT, (WPARAM)0, (LPARAM)&m_toolInfo);
            ::SendMessage(m_hWndToolTip, TTM_TRACKPOSITION, (WPARAM)0, (LPARAM)MAKELONG(rect.right, rect.top));
            ::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)(LPTOOLINFO)&m_toolInfo);
            m_bToolActive = true;
         }
         else
         {
            // text is shorter than client size - no need for tooltip
            ::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)(LPTOOLINFO)&m_toolInfo);
            m_bToolActive = false;
            SetTimer(1, 80, NULL);
         }
      }
   }
   else {
      ::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)(LPTOOLINFO)&m_toolInfo);
      m_bToolActive = false;
      SetTimer(1, 80, NULL);
   }
}


void CColorComboList::OnTimer(UINT_PTR nIDEvent)
{
   CListBox::OnTimer(nIDEvent);

   if (m_hWndToolTip == NULL)
      return;

   CPoint point;
   ::GetCursorPos(&point);
   ScreenToClient(&point);

   CRect rectClient;
   GetClientRect(&rectClient);

   if (!rectClient.PtInRect(point) || 0 == (GetStyle() & WS_VISIBLE))
   {
      KillTimer(nIDEvent); // kill the timer when we exit the listbox
      ::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)(LPTOOLINFO)&m_toolInfo);
      m_bToolActive = false;
   }
}

bool CColorComboList::AddToolTip()
{
   // create a handle to the tooltip window
   m_hWndToolTip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
      TTS_NOPREFIX | TTS_ALWAYSTIP,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      NULL, NULL,
      NULL, NULL);


   if (!m_hWndToolTip)
   {
      return false;
   }


   // Associate the tooltip with the tool.
   memset(&m_toolInfo, 0, sizeof(m_toolInfo));
   m_toolInfo.cbSize = sizeof(TTTOOLINFOA_V2_SIZE);
   m_toolInfo.hwnd = m_hWnd;
   m_toolInfo.uFlags = TTF_TRACK | TTF_TRANSPARENT;
   ::SendMessage(m_hWndToolTip, TTM_SETMAXTIPWIDTH, (WPARAM)0, (LPARAM)SHRT_MAX);
   ::SendMessage(m_hWndToolTip, TTM_ADDTOOL, 0, (LPARAM)&m_toolInfo);

   return true;
}

void CColorComboList::PreSubclassWindow()
{
   if (!AddToolTip())
      m_hWndToolTip = NULL;

   CListBox::PreSubclassWindow();
}
