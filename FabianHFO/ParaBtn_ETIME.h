#pragma once
#include "parabtn.h"

class CViewParaBtn;

class CParaBtn_ETIME :public CParaBtn
{
public:
	explicit CParaBtn_ETIME(CViewParaBtn* parent, BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_ETIME();
	//virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);


public:

	// Operations
public:
	virtual void Draw(int nState);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDParaBtn)
public:
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	static CViewParaBtn* VIEWPARAPTN;

	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_ETIME)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};




