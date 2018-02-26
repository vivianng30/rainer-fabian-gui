/**********************************************************************************************//**
 * \file	UDBtn.h.
 *
 * Declares the ud button class
 **************************************************************************************************/

#pragma once

/***** Header **************************************************************/
#include "Btn.h"
#include "globdefs.h"
//#include "../MVViewHandler.h"

#define WM_UPDATE_UDBTN			(WM_USER+0x0011)
#define LIST_ITEM_MAX_TEXT		256

/***** Global **************************************************************/
//typedef struct tagLIST_ITEM
//{
//	int nNbr;
//	TCHAR tText[LIST_ITEM_MAX_TEXT];
//}LIST_ITEM;
//typedef struct tagLIST
//{
//	LIST_ITEM* pli;
//	int nSize;
//}LIST;
//typedef struct tagVALUE
//{
//	int nValue;
//	int nUpperLimit;
//	int nLowerLimit;
//	TCHAR tText[16];
//}VALUE;
// CUDBtn

class CUDBtn : public CButton
{
	DECLARE_DYNAMIC(CUDBtn)

public:
	CUDBtn(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual BOOL Create(CWnd* pParentWnd, HFONT hFontText, HFONT hFontValue, VALUE v);
	//virtual BOOL Create(CWnd* pParentWnd, HFONT hFont, LIST* pl, int nValue);

	inline void GetButtonColors(BTNCOLORS* pbtncr){memcpy(pbtncr,&m_btncr,sizeof(BTNCOLORS));};
	inline void SetButtonColors(BTNCOLORS* pbtncr){memcpy(&m_btncr,pbtncr,sizeof(BTNCOLORS));};

	void SetText(TCHAR* pszText,bool bTextOnly=false);
	void SetText(CStringW& szText,bool bTextOnly=false);
	void RefreshText(CStringW& szText,bool bTextOnly=false);
	void DrawDirectDown(void);

	void SetColors(COLORREF cr, COLORREF crDisabel=0x00828282);

	void SetValue(VALUE v, bool bRedraw=false);
	inline void GetValue(VALUE* pv){memcpy(pv,&m_v,sizeof(VALUE));};
	//void SetList(LIST* pl, int nValue, bool bRedraw);

	bool GetButton(int* pnValue=NULL);
	bool SetLimits(int nLower,int nUpper);

	inline void SetTextFormat(DWORD dwFormat){m_btn.dwFormat=dwFormat;};
	inline void SetDepress(void){m_bButtonIsActive=true;};

	void SetUDKeys(WORD kUP, WORD kDown, WORD kSpace);

	bool DisableText(bool bDisable=true);

	void DrawDirect(void);
	// Attributes
private:
	BTNCOLORS m_btncr;
	BTN m_btn;
	HDC m_hDC;
	HBITMAP m_hBm;
	HBITMAP m_hBmPrev;
	HFONT m_hFontText;
	HFONT m_hFontValue;
	RECT m_rcClient;
	TCHAR m_pszText[32];
	bool m_bButtonIsActive;
	bool m_bTextOnly;
	VALUE m_v;
	int m_nOffset;
	//LIST* m_pl;
	bool m_bDisableText;
	bool m_bScrollOver;
	WORD m_kUp;
	WORD m_kDown;
	WORD m_kSpace;
public:

	// Operations
public:
	void Draw(int nState);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDBtn)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CUDBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUDBtn)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


