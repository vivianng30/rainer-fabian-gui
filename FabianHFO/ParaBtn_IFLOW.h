/**********************************************************************************************//**
 * \file	ParaBtn_IFLOW.h.
 *
 * Declares the para button iflow class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

class CParaBtn_IFLOW :public CParaBtn
{
public:
	explicit CParaBtn_IFLOW(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_IFLOW();
	//virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);


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
	//{{AFX_MSG(CParaBtn_IFLOW)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};





