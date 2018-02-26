/**********************************************************************************************//**
 * \file	ParaBtn_VLIMIT.h.
 *
 * Declares the para button vlimit class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

class CParaBtn_VLIMIT :public CParaBtn
{
public:
	explicit CParaBtn_VLIMIT(BTN &btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_VLIMIT();
	//virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);


public:

	// Operations
public:
	virtual void Draw(int nState);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParaBtn)
protected:
	
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_VLIMIT)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

