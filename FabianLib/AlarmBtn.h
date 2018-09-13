/**********************************************************************************************//**
 * \file	AlarmBtn.h.
 *
 * Declares the alarm button class
 **************************************************************************************************/

#pragma once
#include "Bmp.h"
#include "Btn.h"

/***** Preprocessor ********************************************************/
#define BTN_DW_FOCUS	0x0001
#define BTN_DW_UP		0x0002
#define BTN_DW_DW		0x0003
#define BTN_DW_DIS		0x0004


// CAlarmBtn

class CAlarmBtn : public CButton
{
	DECLARE_DYNAMIC(CAlarmBtn)

public:
	//CAlarmBtn();
	explicit CAlarmBtn(const BTN &btn,COLORREF cr=0x00000000, bool bTwoLine=false);
	virtual ~CAlarmBtn();


	inline void GetButtonColors(BTNCOLORS* pbtncr){memcpy(pbtncr,&m_btncr,sizeof(BTNCOLORS));};
	inline void SetButtonColors(BTNCOLORS* pbtncr){memcpy(&m_btncr,pbtncr,sizeof(BTNCOLORS));};

	void SetText(TCHAR* pszText, int nNbr=-1,bool bRefresh=false);
	void SetText(const CStringW& szText, int nNbr=-1,bool bRefresh=false);
	void RefreshText(const CStringW& szText, int nNbr=-1);
	void RefreshText(TCHAR* pszText, int nNbr=-1);
	void SetChar(TCHAR t);
	void SetText(TCHAR* pszTextTop,TCHAR* pszTextBottom,int nNbr=-1,bool bRefresh=false);
	void RefreshText(TCHAR* pszTextTop,TCHAR* pszTextBottom,int nNbr=-1);
	void SetText(const CStringW& pszTextTop,const CStringW& pszTextBottom,int nNbr=-1,bool bRefresh=false);
	void RefreshText(CStringW& pszTextTop,CStringW& pszTextBottom,int nNbr=-1);


	void SetBitmaps(CBmp*	pcBmpUp,
		CBmp*	pcBmpDown,
		CBmp*	pcBmpDisabled,
		CBmp*	pcBmpFocus);

protected:
	void Draw(int nState);
	// Attributes
private:

	HDC m_hDC;
	HBITMAP m_hBmp;
	HBITMAP m_hBmpPrev;
	HFONT m_hFont;

	BTNCOLORS m_btncr;
	BTN m_btn;
	RECT m_rcClient;

	int m_nNbr;
	int m_nXOffset;

	TCHAR m_pszText[MAX_PATH];
	TCHAR m_pszTextBottom[MAX_PATH];
	TCHAR m_pszTextTop[MAX_PATH];

	bool m_bTwoLine;
	bool m_bLMouseButtonDown;

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlarmBtn)
public:
	virtual BOOL Create(CWnd* pParentWnd, HFONT hFont, int nXOffset=0,DWORD	dwStyle = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW/*|WS_TABSTOP*/);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

	// Implementation

	// Generated message map functions
protected:
	//{{AFX_MSG(CAlarmBtn)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point); 
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnSetFocus(CWnd* pOldWnd);
};




