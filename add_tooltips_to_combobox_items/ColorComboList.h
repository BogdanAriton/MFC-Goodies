#if !defined(COLORCOMBO_LIST)
#define COLORCOMBO_LIST

#pragma once
#include <afxwin.h>

/// @class CColorComboList - this represents the list box that is created when we drop down the ColorCombo
class CColorComboList : public CListBox
{
public:
   /// Constructor
   CColorComboList();
   /// default destructor
   virtual ~CColorComboList() = default;
   DECLARE_MESSAGE_MAP()
   /// Tracking the mouse movement so that we know when we enter the listbox rectangle
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   /// Capture the timer event so that we can stop the timer when we leave the client rectangle of the listbox
   afx_msg void OnTimer(UINT_PTR nIDEvent);
   /// Override the PreSubclassWindow event call AddToolTip
   virtual void PreSubclassWindow();
   
protected:
   /// Flag that let's us know if we entered the listbox and if the tooltip should be active or not
   bool m_bToolActive;
   /// A handle to the tooltip window
   HWND m_hWndToolTip;
   /// The tooltip info (used to change the text based on position)
   TOOLINFO m_toolInfo;
   /// Create the tooltip window, the tooltip info and save the handle to the tooltip window
   bool AddToolTip();

private:
   /// Returns a multiline tooltip that will be easier to read
   static const std::string ReturnMultilineToolTip(const CString& toolTipText);

   /// Splitting the email list into groups of m_iNoEmailsPerLine
   static const std::string SplitByEmail(const std::string& emailList);

   /// Number of emails per line
   static const int m_iNoEmailsPerLine = 5;

   /// Friend class CColorCombo will be able to access the private members of CColorComboList
   friend class CColorCombo;

};

#endif