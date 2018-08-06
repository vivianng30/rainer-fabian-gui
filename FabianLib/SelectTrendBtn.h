/**********************************************************************************************//**
 * \file	SelectTrendBtn.h.
 *
 * Declares the select trend button class
 **************************************************************************************************/

#pragma once
#include "Btn.h"



// CSelectTrendBtn

class CSelectTrendBtn : public CBtn
{
	DECLARE_DYNAMIC(CSelectTrendBtn)

public:
	//CSelectTrendBtn();
	CSelectTrendBtn(BTN btn,COLORREF cr);
	virtual ~CSelectTrendBtn();

	//virtual BOOL PreTranslateMessage(MSG* pMsg);

	bool IsDepressed( void );
	bool Depress(bool bDown);
	void DrawDirectUp(void);
	void LoadDataRunning(bool state);


	void SetText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr=-1);
	void RefreshText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr=-1);
	void SetText(CStringW& pszText,CStringW& pszTextDown,int nNbr=-1);
	void RefreshText(CStringW& pszText,CStringW& pszTextDown,int nNbr=-1);
	//inline void SetFocusDownBmp(CBmp* pcBmpFocusDown){m_pcBmpFocusDown=pcBmpFocusDown;};

	int GetBtnId();

	/*void SetBitmaps(CBmp*	pcBmpUp,
		CBmp*	pcBmpDown,
		CBmp*	pcBmpDisabled,
		CBmp*	pcBmpFocus);*/

protected:
	void Draw(int nState,int AddInfo=0);
	void DrawRotatedText(CDC* pDC, const CStringW str, CRect rect, double angle, UINT nOptions = 0);

	// Attributes
public:
private:
	bool m_bDepressed;
	bool m_bLMouseButtonDown;
	bool m_bLButtonDown;
	CBmp* m_pcBmpFocusDown;

	bool m_bLoadDataRunning;

	//bool m_bButtonIsActive;
protected:
	TCHAR m_pszTextDown[MAX_PATH];
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
	//{{AFX_MSG(CSelectTrendBtn)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnSetFocus(CWnd* pOldWnd);
	//afx_msg void OnKillFocus(CWnd* pNewWnd);
};


