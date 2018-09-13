/**********************************************************************************************//**
 * \file	UpDwnBtn.h.
 *
 * Declares the up dwn button class
 **************************************************************************************************/

#pragma once
#include "Btn.h"


// CMenuBtn

class CUpDwnBtn : public CBtn
{
	DECLARE_DYNAMIC(CUpDwnBtn)

public:
	CUpDwnBtn(BTN btn,COLORREF cr, bool bTwoLine=false, bool bVerticalTxt=false);
	virtual ~CUpDwnBtn();

	bool IsDepressed( void );
	bool Depress(bool bDown);
	void DrawDirectUp(void);
	

	void SetText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr=-1);
	void RefreshText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr=-1);
	void SetText(const CStringW& pszText,const CStringW& pszTextDown,int nNbr=-1);
	void RefreshText(CStringW& pszText,CStringW& pszTextDown,int nNbr=-1);
	void SetText(CStringW& pszTextTop,CStringW& pszTextBottom,CStringW& pszTextTopDown,CStringW& pszTextBottomDown,int nNbr=-1);
	void RefreshText(CStringW& pszTextTop,CStringW& pszTextBottom,CStringW& pszTextTopDown,CStringW& pszTextBottomDown,int nNbr=-1);

	int GetBtnId();

protected:
	void Draw(int nState, int AddInfo=0);
	// Attributes
public:
private:
	//bool m_bDepressed;
	bool m_bLMouseButtonDown;
	//bool m_bLButtonDown;
	CBmp* m_pcBmpFocusDown;

	bool m_bButtonIsActive;
protected:
	TCHAR m_pszTextDown[MAX_PATH];

	TCHAR m_pszTextBottom[MAX_PATH];
	TCHAR m_pszTextTop[MAX_PATH];
	TCHAR m_pszTextBottomDown[MAX_PATH];
	TCHAR m_pszTextTopDown[MAX_PATH];

	bool m_bTwoLine;
	bool m_bVerticalTxt;
	//CMVViewHandler* m_pViewHandler;
	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpDwnBtn)
public:
	//	virtual BOOL Create(CWnd* pParentWnd, HFONT hFont);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CUpDwnBtn)
	//afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


