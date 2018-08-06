/**********************************************************************************************//**
 * \file	ParaBtn_O2.h.
 *
 * Declares the para button o 2 class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

class CViewParaBtn;

class CParaBtn_O2 :public CParaBtn
{
public:
	explicit CParaBtn_O2(CViewParaBtn* parent, BTN &btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_O2();
	//virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);

	
public:

	// Operations
public:
	virtual void Draw(int nState);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParaBtn_O2)
public:
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
protected:
	static CViewParaBtn* VIEWPARAPTN;

	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_O2)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



