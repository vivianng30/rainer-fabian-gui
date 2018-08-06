/**********************************************************************************************//**
 * \file	PresetMenuBtn.h.
 *
 * Declares the preset menu button class
 **************************************************************************************************/

#pragma once
#include "Btn.h"


// CPresetMenuBtn

class CPresetMenuBtn : public CBtn
{
	DECLARE_DYNAMIC(CPresetMenuBtn)

	

public:
	CPresetMenuBtn(BTN btn,COLORREF cr);
	virtual ~CPresetMenuBtn();

	enum eBtnState{
		UP			= 0, 
		PRESET		= 1, 
		DOWN		= 2
	};

	//bool IsDepressed( void );
	//bool Depress(bool bDown);
	void DrawDirectUp(void);
	//void DrawDirectDown(void);
	//void DrawDirectPreset(void);
	virtual void SetText(TCHAR* pszText,/*TCHAR* pszTextDown,*/int nNbr=-1);
	void RefreshText(TCHAR* pszText,/*TCHAR* pszTextDown,*/int nNbr=-1);
	void SetText(CStringW& pszText,/*CStringW& pszTextDown,*/int nNbr=-1);
	void RefreshText(CStringW& pszText,/*CStringW& pszTextDown,*/int nNbr=-1);
	void SetTextTwoLines(CStringW& pszTextTop,CStringW& pszTextBottom,int nNbr=-1);
	void RefreshTextTwoLines(CStringW& pszTextTop,CStringW& pszTextBottom,int nNbr=-1);
	inline void SetFocusDownBmp(CBmp* pcBmpFocusDown){m_pcBmpFocusDown=pcBmpFocusDown;};

	int GetBtnId();

	eBtnState GetBtnState();
	void SetBtnState(eBtnState state, bool bReleaseButton=false);

protected:
	void Draw(int nState,int AddInfo=0);
	// Attributes
public:
private:
	eBtnState m_eBtnState;
	//bool m_bDepressed;
	bool m_bLMouseButtonDown;
	bool m_bLButtonDown;
	CBmp* m_pcBmpFocusDown;

	bool m_bTwoLine;
protected:
	//TCHAR m_pszTextDown[MAX_PATH];
	TCHAR m_pszTextTop[MAX_PATH];
	TCHAR m_pszTextBottom[MAX_PATH];
	//CMVViewHandler* m_pViewHandler;
	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPresetMenuBtn)
public:
	//	virtual BOOL Create(CWnd* pParentWnd, HFONT hFont);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CPresetMenuBtn)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnSetFocus(CWnd* pOldWnd);
};



