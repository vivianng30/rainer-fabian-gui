// XComboList.h
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////
#include "colour.h"
#include "ColorListBox.h"

#ifndef XCOMBOLIST_H
#define XCOMBOLIST_H

extern UINT NEAR WM_XCOMBOLIST_VK_RETURN;
extern UINT NEAR WM_XCOMBOLIST_VK_ESCAPE;
extern UINT NEAR WM_XCOMBOLIST_KEYDOWN;
extern UINT NEAR WM_XCOMBOLIST_LBUTTONUP;

/////////////////////////////////////////////////////////////////////////////
// CXComboList window

class CXComboList : public CWnd
{
protected:
	struct LItem
	{
		COLORREF	cBg,	cFg;
		UINT		uHal,	uVal;
		BOOL		bActive;
		DWORD		dw;
	};
// Construction
public:
	CXComboList(CWnd *pParent);
	virtual ~CXComboList();

// Attributes
public:

// Operations
public:
	void SetActive(/*int nScrollBarWidth*/);

	int AddItem(CStringW lpszItem,
		COLORREF bg=DEF_COL,	  COLORREF fg=DEF_COL,
		UINT hal=DT_LEFT, UINT val=DT_VCENTER,
		BOOL active=TRUE, DWORD res=NULL);
	

	int AddHead(LPCTSTR lpszItem,
		COLORREF bg=DEF_COL,	  COLORREF fg=DEF_COL,
		UINT hal=DT_LEFT, UINT val=DT_VCENTER,
		BOOL active=TRUE, DWORD res=NULL);
	

	int AddString(LPCTSTR lpszItem)
	{
		//int iTemp = m_ListBox.AddString(lpszItem);

		int iTemp = m_ListBox.AddItem(lpszItem,PATALARM_COL,BLACK);

		return iTemp;
	}
	int GetCount()
	{
		return m_ListBox.GetCount();
	}
	void GetText(int nIndex, CStringW& rString)
	{
		m_ListBox.GetText(nIndex, rString);
	}
	int FindStringExact(int nIndexStart, LPCTSTR lpszFind)
	{
		return m_ListBox.FindStringExact(nIndexStart, lpszFind);
	}
	int SetCurSel(int nSelect)
	{
		return m_ListBox.SetCurSel(nSelect);
	}
	int GetCurSel()
	{
		return m_ListBox.GetCurSel();
	}
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE)
	{
		m_ListBox.SetFont(pFont, bRedraw);
	}

	void SetHorizontalExtent(/*CListBox& ListBox*/);

	void DeleteString(int idx);
	void InvalidateListBox();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXComboList)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual CScrollBar* GetScrollBarCtrl(int nBar);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CColorListBox	m_ListBox;
	//CListBox	m_ListBox;
	CScrollBar	m_wndSBVert;
	CWnd *		m_pParent;
	int			m_nCount;
	BOOL		m_bFirstTime;

	void SendRegisteredMessage(UINT nMsg, WPARAM wParam, LPARAM lParam);

	// Generated message map functions
protected:
	//{{AFX_MSG(CXComboList)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XCOMBOLIST_H
