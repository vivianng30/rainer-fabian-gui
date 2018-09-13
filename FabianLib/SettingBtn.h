/**********************************************************************************************//**
 * \file	SettingBtn.h.
 *
 * Declares the setting button class
 **************************************************************************************************/

#pragma once
#include "Bmp.h"
#include "Btn.h"
#include "MVModel.h"

/***** Preprocessor ********************************************************/
#define BTN_DW_FOCUS	0x0001
#define BTN_DW_UP		0x0002
#define BTN_DW_DW		0x0003
#define BTN_DW_DIS		0x0004


// CSelectSetupBtn

class CSettingBtn : public CButton
{
	DECLARE_DYNAMIC(CSettingBtn)

public:
	CSettingBtn(BTN btn,COLORREF cr=0x00000000, bool bValue = false);
	virtual ~CSettingBtn();

	//virtual BOOL PreTranslateMessage(MSG* pMsg);

	inline void GetButtonColors(BTNCOLORS* pbtncr){memcpy(pbtncr,&m_btncr,sizeof(BTNCOLORS));};
	inline void SetButtonColors(BTNCOLORS* pbtncr){memcpy(&m_btncr,pbtncr,sizeof(BTNCOLORS));};

	virtual void DrawDirectUp();
	virtual void DrawDirectDown();
	virtual void Redraw();

	void SetText(TCHAR* pszText, int nNbr=-1);
	void SetText(const CStringW& szText, int nNbr=-1);
	void SetValueText(TCHAR* pszText, int nNbr=-1);
	void SetValueText(CStringW& szText, int nNbr=-1);
	void RefreshText(const CStringW& szText, int nNbr=-1);
	void RefreshValueText(TCHAR* pszText, int nNbr=-1);
	void RefreshValueText(CStringW& szText, int nNbr=-1);
	void RefreshText(TCHAR* pszText, int nNbr=-1);
	void SetChar(TCHAR t);
	void SetState(eBtnState state);
	eBtnState GetState();

	void DrawOK(bool state);
	void LoadFlag(CStringW csFlag);

	/*bool IsDepressed( void );
	bool Depress(bool bDown);*/

	int GetBtnId();

	void SetBitmaps(CBmp*	pcBmpUp,
		CBmp*	pcBmpDown,
		CBmp*	pcBmpDisabled,
		CBmp*	pcBmpFocus);

	void SetLangString(CStringW szLang);
	CStringW GetLangString();

protected:
	//void WriteCurrentValue();
	void Draw(int nState/*, TCHAR* psz=NULL*/);
	//void DrawRotatedText(CDC* pDC, const CStringW str, CRect rect, double angle, UINT nOptions = 0);

	CMVModel *getModel();

	// Attributes
public:
private:
	bool m_bLMouseButtonDown;
protected:
	CMVModel* m_pModel;

	eBtnState m_btnState;

	BTNCOLORS m_btncr;
	BTN m_btn;
	HDC m_hDC;
	HBITMAP m_hBm;
	HBITMAP m_hBmPrev;
	HFONT m_hFont;
	RECT m_rcClient;
	TCHAR m_pszText[MAX_PATH];
	TCHAR m_pszValueText[MAX_PATH];
	int m_nNbr;
	int m_nXOffset;

	
	WORD m_kUp;
	WORD m_kDown;
	WORD m_kSpace;

	bool m_bDepressed;
	bool m_bLButtonDown;
	

	bool m_bValue;

	CBmp* m_pcOkFc;
	CBmp* m_pcOkUp;
	CBmp* m_pcOkDw;

	CBmp* m_pbmpFlag;

	bool m_bDrawOK;

	CStringW m_szLanguageString;


	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingBtn)
public:
	virtual BOOL Create(CWnd* pParentWnd, HFONT hFont, int nXOffset=0,DWORD	dwStyle = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW/*|WS_TABSTOP*/);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

	// Implementation

	// Generated message map functions
protected:
	//{{AFX_MSG(CSettingBtn)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	/*afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);*/
	//afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	/*afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);*/
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnSetFocus(CWnd* pOldWnd);
};


