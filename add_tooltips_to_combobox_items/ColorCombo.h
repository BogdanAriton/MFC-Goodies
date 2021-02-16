#if !defined(AFX_COLORCOMBO_H__C3B20B12_D3B7_403F_8F46_B3AA4BE910AB__INCLUDED_)
#define AFX_COLORCOMBO_H__C3B20B12_D3B7_403F_8F46_B3AA4BE910AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "ColorComboList.h"

/// @class CColorCombo
class CColorCombo : public CComboBox
{
public:
   /// Constructor
   CColorCombo(COleControl* pCtrl);
   /// Destructor
   virtual ~CColorCombo();
   /// Capture the background color of the control
   COLORREF m_background;
   /// The brush that will paint the selected color
   CBrush m_brush;
   /// Structure that holds the combo information - we take the listbox handle from it
   COMBOBOXINFO m_comboInfo;
   /// Defines the tooltip info
   TOOLINFO m_toolInfo;
   /// Handle to the tooltip window
   HWND m_hWndToolTip;
   /// Handle to the combo box list
   HWND m_hwndList;
   /// The listbox object that is linked to the combo list
   CColorComboList m_ListBox;

   /// Track if the DBPrint control had focus
   bool m_pCtrlHasFocus;
   /// Updates the m_pCtrlHasFocus with focus status
   void PCtrlGotFocus();
   /// Gets the accessible name for this control
   virtual HRESULT get_accName(VARIANT varChild, BSTR *pszName);
   /// SetBackColor
   void SetBackColor(COLORREF color);
   /// Override on the create event - we initialize the tooltip and the COMBOBOXINFO
   virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

private:
   /// Flag that tells us whether the tooltip should be active or not
   bool m_bToolActive;
   /// Creates a handle to the tooptip window and creates the tooltip info
   bool AddToolTip(int comboID, HWND hwndCombo, HWND hDlg, PTSTR pszText);

protected:
   /// Reference to the DBPrint control
   COleControl* m_pCtrl;
   /// Override over OnCtlColor event to set the background color
   afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
   /// Override over CtlColor event to set the background color
   afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
   /// Override on OnSetFocus event to set the focus on DBPrint control
   afx_msg void OnSetFocus(CWnd* pOldWnd);
   /// Override on OnKillFocus event to capture the fact that focus was lost
   afx_msg void OnKillFocus(CWnd* pNewWnd);
   /// OVerride on OnMouseMove event to update the tooltip text based on where the mosue hovers
   void OnMouseMove(UINT nFlags, CPoint point);
   /// Override on OnLButtonDown to send an message that will be captured on the DBPrint control to open the address book when left mouse button is pressed
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   /// The timer event will keep track of the mouse position and kill the timer if we leave the combo
   afx_msg void OnTimer(UINT_PTR nIDEvent);
   DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_COLORCOMBO_H__C3B20B12_D3B7_403F_8F46_B3AA4BE910AB__INCLUDED_)
