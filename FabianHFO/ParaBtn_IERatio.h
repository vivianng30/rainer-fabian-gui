/**********************************************************************************************//**
 * \file	ParaBtn_IERatio.h.
 *
 * Declares the para button IE ratio class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

class CParaBtn_IERatio :
	public CParaBtn
{
public:
	explicit CParaBtn_IERatio(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_IERatio();
	//virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);


public:

	// Operations
public:
	virtual void Draw(int nState);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParaBtn)
public:
	//}}AFX_VIRTUAL



	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_IERatio)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};