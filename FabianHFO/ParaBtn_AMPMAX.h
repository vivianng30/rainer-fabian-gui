/**********************************************************************************************//**
 * \file	ParaBtn_AMPMAX.h.
 *
 * Declares the para button ampmax class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

// CParaBtn_AMPMAX

class CParaBtn_AMPMAX : public CParaBtn
{
public:
	explicit CParaBtn_AMPMAX(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_AMPMAX();


public:

	// Operations
public:
	virtual void Draw(int nState);
	virtual void SetValue(fVALUE v, bool bRedraw=false);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParaBtn)
public:
	//}}AFX_VIRTUAL




	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_AMPMAX)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


