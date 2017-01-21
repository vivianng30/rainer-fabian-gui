#pragma once
#include "Bmp.h"
#include "Btn.h"

/***** Preprocessor ********************************************************/
#define BTN_DW_FOCUS	0x0001
#define BTN_DW_UP		0x0002
#define BTN_DW_DW		0x0003
#define BTN_DW_DIS		0x0004



class CTrendScaleBtn : public CButton
{
	DECLARE_DYNAMIC(CTrendScaleBtn)

public:
	CTrendScaleBtn(BTN btn,COLORREF cr=0x00000000, bool bTwoLine=false);
	virtual ~CTrendScaleBtn();


	inline void GetButtonColors(BTNCOLORS* pbtncr){memcpy(pbtncr,&m_btncr,sizeof(BTNCOLORS));};
	inline void SetButtonColors(BTNCOLORS* pbtncr){memcpy(&m_btncr,pbtncr,sizeof(BTNCOLORS));};

	void SetText(TCHAR* pszText, int nNbr=-1);
	void SetText(CStringW& szText, int nNbr=-1);
	void RefreshText(CStringW& szText, int nNbr=-1);
	void RefreshText(TCHAR* pszText, int nNbr=-1);
	void SetChar(TCHAR t);
	void SetText(TCHAR* pszTextTop,TCHAR* pszTextBottom,int nNbr=-1);
	void RefreshText(TCHAR* pszTextTop,TCHAR* pszTextBottom,int nNbr=-1);
	void SetText(CStringW& pszTextTop,CStringW& pszTextBottom,int nNbr=-1);
	void RefreshText(CStringW& pszTextTop,CStringW& pszTextBottom,int nNbr=-1);

	void DrawDirectUp();

	void SetBitmaps(CBmp*	pcBmpUp,
		CBmp*	pcBmpDown,
		CBmp*	pcBmpDisabled,
		CBmp*	pcBmpFocus);

protected:
	void Draw(int nState/*, TCHAR* psz=NULL*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
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
	TCHAR m_pszTextBottom[MAX_PATH];
	TCHAR m_pszTextTop[MAX_PATH];
	int m_nNbr;
	int m_nXOffset;
	bool m_bTwoLine;

	bool m_bButtonDown;

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrendScaleBtn)
public:
	virtual BOOL Create(CWnd* pParentWnd, HFONT hFont, int nXOffset=0,DWORD	dwStyle = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW/*|WS_TABSTOP*/);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

	// Implementation

	// Generated message map functions
protected:
	//{{AFX_MSG(CTrendScaleBtn)
	/*afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point); 
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);*/
	//afx_msg void OnSetFocus(CWnd* pOldWnd);
	//afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	
};






