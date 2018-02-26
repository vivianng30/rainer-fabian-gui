/**********************************************************************************************//**
 * \file	ParaBtn_TRIGGERNMODE.h.
 *
 * Declares the para button triggernmode class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

// CParaBtn_TRIGGERNMODE

class CParaBtn_TRIGGERNMODE : public CParaBtn
{
public:
	CParaBtn_TRIGGERNMODE(BTN btn, int nOffset=0,bool bScrollOver=true);
	
	virtual ~CParaBtn_TRIGGERNMODE();
public:

	void Trigger();

	// Operations
public:
	virtual void Draw(int nState);
protected:
	//{{AFX_MSG(CParaBtn_TRIGGERNMODE)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};


