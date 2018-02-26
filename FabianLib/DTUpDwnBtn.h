/**********************************************************************************************//**
 * \file	DTUpDwnBtn.h.
 *
 * Declares the dt up dwn button class
 **************************************************************************************************/

#pragma once
#include "Btn.h"

// CDTUpDwnBtn
class CDTUpDwnBtn : public CBtn
{
	DECLARE_DYNAMIC(CDTUpDwnBtn)

public:
	CDTUpDwnBtn(BTN &btn,COLORREF cr);
	virtual ~CDTUpDwnBtn();

	bool IsDepressed( void );
	bool Depress(bool bDown);
	void DrawDirectUp(void);
	/*virtual void SetText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr=-1);
	void RefreshText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr=-1);*/
	//inline void SetFocusDownBmp(CBmp* pcBmpFocusDown){m_pcBmpFocusDown=pcBmpFocusDown;};

	int GetBtnId();

protected:
	void Draw(int nState,int AddInfo=0);
	// Attributes
public:
private:
	bool m_bDepressed;
	bool m_bLMouseButtonDown;
	//bool m_bLButtonDown;
	CBmp* m_pcBmpFocusDown;
protected:
	//TCHAR m_pszTextDown[MAX_PATH];
	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDTUpDwnBtn)
public:
	//	virtual BOOL Create(CWnd* pParentWnd, HFONT hFont);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CDTUpDwnBtn)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	
};


