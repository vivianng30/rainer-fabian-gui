#pragma once
#include "parabtn.h"

class CViewParaBtn;

class CParaBtn_PEEP :public CParaBtn
{
public:
	explicit CParaBtn_PEEP(CViewParaBtn* parent, BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_PEEP();
	//virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);

	

private:
	
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
	//{{AFX_MSG(CParaBtn_PEEP)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


