#pragma once
#include "parabtn.h"

// CParaBtn_FLOW

class CParaBtn_FLOW : public CParaBtn
{
	DECLARE_DYNAMIC(CParaBtn_FLOW)

public:
	explicit CParaBtn_FLOW(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_FLOW();



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


	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_FLOW)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};





