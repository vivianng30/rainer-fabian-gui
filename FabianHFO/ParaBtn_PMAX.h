#pragma once
#include "parabtn.h"

// CParaBtn_PMAX

class CParaBtn_PMAX : public CParaBtn
{
public:
	explicit CParaBtn_PMAX(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_PMAX();
	//virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);


public:

	// Operations
public:
	virtual void Draw(int nState);
	virtual void SetValue(fVALUE v, bool bRedraw=false);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParaBtn)
public:
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL




	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_PMAX)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


