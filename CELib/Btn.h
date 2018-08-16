#pragma once
#include "Bmp.h"
//#include "../MVViewHandler.h"

/***** Preprocessor ********************************************************/
#define BTN_DW_FOCUS	0x0001
#define BTN_DW_UP		0x0002
#define BTN_DW_DW		0x0003
#define BTN_DW_DIS		0x0004

/***** Global/Extern *******************************************************/
typedef struct tagBTN
{
	CBmp*		pcBmpUp;
	CBmp*		pcBmpDown;
	CBmp*		pcBmpDisabled;
	CBmp*		pcBmpFocus;
	DWORD		dwFormat;
	POINT		poPosition;
	WORD		wID;
	//	COLORREF	crBkg;
}BTN;


typedef struct tagBTNCOLORS
{
	COLORREF	crTxtUp;
	COLORREF	crTxtDown;
	COLORREF	crSubTxtDown;
	COLORREF	crTxtDisabled;
	COLORREF	crTxtFocus;
	COLORREF	crBkg;
}BTNCOLORS;
// CBtn

class CBtn : public CButton
{
	DECLARE_DYNAMIC(CBtn)

public:
	CBtn();
	CBtn(BTN &btn,COLORREF crTxtUp=0x00000000, COLORREF crTxtDown=0x00000000, COLORREF crTxtDisabled=0x00000000, COLORREF crTxtFocus=0x00000000);
	virtual void Initialize(BTN &btn,COLORREF crTxtUp=0x00000000, COLORREF crTxtDown=0x00000000, COLORREF crTxtDisabled=0x00000000, COLORREF crTxtFocus=0x00000000);

	inline void GetButtonColors(BTNCOLORS* pbtncr){memcpy(pbtncr,&m_btncr,sizeof(BTNCOLORS));};
	inline void SetButtonColors(BTNCOLORS* pbtncr){memcpy(&m_btncr,pbtncr,sizeof(BTNCOLORS));};

	void ReplaceFont(HFONT hFont);

	void SetText(TCHAR* pszText, int nNbr=-1);
	void SetText(CStringW& szText, int nNbr=-1);
	void RefreshText(CStringW& szText, int nNbr=-1);
	void SetChar(TCHAR t);

	void SetBitmaps(CBmp*	pcBmpUp,
		CBmp*	pcBmpDown,
		CBmp*	pcBmpDisabled,
		CBmp*	pcBmpFocus);

	
protected:
	void Draw(int nState/*, TCHAR* psz=NULL*/);
	// Attributes
public:
private:
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
	int m_nYOffset;
	bool m_bMoveDepressedText;
	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBtn)
public:
	virtual BOOL Create(CWnd* pParentWnd, HFONT hFont, int nXOffset=0, int nYOffset=0, bool bMoveDepressedText=true);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBtn)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point); 
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


