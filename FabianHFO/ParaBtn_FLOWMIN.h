
#pragma once
#include "parabtn.h"

class CParaBtn_FLOWMIN :public CParaBtn
{
public:
	explicit CParaBtn_FLOWMIN(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_FLOWMIN();
	//virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);


public:

	// Operations
public:
	virtual void Draw(int nState);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDParaBtn)
public:
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_FLOWMIN)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};






