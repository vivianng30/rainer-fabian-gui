/**********************************************************************************************//**
 * \file	SelectGraphBtn.h.
 *
 * Declares the select graph button class
 **************************************************************************************************/

#pragma once
#include "Bmp.h"
#include "Btn.h"

/***** Preprocessor ********************************************************/
#define BTN_DW_FOCUS	0x0001
#define BTN_DW_UP		0x0002
#define BTN_DW_DW		0x0003
#define BTN_DW_DIS		0x0004


// CSelectGraphBtn

class CSelectGraphBtn : public CButton
{
	DECLARE_DYNAMIC(CSelectGraphBtn)

public:
	//CSelectGraphBtn();
	CSelectGraphBtn(BTN btn,COLORREF crUp=0x00000000,COLORREF crDw=0x00000000,COLORREF crFc=0x00000000);
	virtual ~CSelectGraphBtn();

	//virtual BOOL PreTranslateMessage(MSG* pMsg);

	inline void GetButtonColors(BTNCOLORS* pbtncr){memcpy(pbtncr,&m_btncr,sizeof(BTNCOLORS));};
	inline void SetButtonColors(BTNCOLORS* pbtncr){memcpy(&m_btncr,pbtncr,sizeof(BTNCOLORS));};

	void SetText(TCHAR* pszText, int nNbr=-1);
	void SetText(const CStringW& szText, int nNbr=-1);
	void RefreshText(const CStringW& szText, int nNbr=-1);
	void RefreshText(TCHAR* pszText, int nNbr=-1);
	void SetChar(TCHAR t);


	void SetBitmaps(CBmp*	pcBmpUp,
		CBmp*	pcBmpDown,
		CBmp*	pcBmpDisabled,
		CBmp*	pcBmpFocus);

protected:
	void Draw(int nState/*, TCHAR* psz=NULL*/);
	void DrawRotatedText(CDC* pDC, const CStringW str, CRect rect, double angle, UINT nOptions = 0);

	// Attributes
public:
private:
	//bool m_bLMouseButtonDown;
protected:
	BTNCOLORS m_btncr;
	BTN m_btn;
	HDC m_hDC;
	HBITMAP m_hBm;
	HBITMAP m_hBmPrev;
	HFONT m_hFont;
	RECT m_rcClient;
	TCHAR m_pszText[MAX_PATH];
	int m_nNbr;
	int m_nXOffset;

	WORD m_kUp;
	WORD m_kDown;
	WORD m_kSpace;
	

	//CMVViewHandler* m_pViewHandler;
	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectGraphBtn)
public:
	virtual BOOL Create(CWnd* pParentWnd, HFONT hFont, int nXOffset=0,DWORD	dwStyle = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW/*|WS_TABSTOP*/);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

	// Implementation

	// Generated message map functions
protected:
	//{{AFX_MSG(CSelectGraphBtn)
	/*afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point); 
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);*/
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnSetFocus(CWnd* pOldWnd);
};


