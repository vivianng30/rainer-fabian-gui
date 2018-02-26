/**********************************************************************************************//**
 * \file	ParaBtn_HFFREQ.h.
 *
 * Declares the para button hffreq class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

class CParaBtn_HFFREQ :public CParaBtn
{
public:
	explicit CParaBtn_HFFREQ(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_HFFREQ();


public:

	// Operations
public:
	virtual void Draw(int nState);

	


	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_HFFREQ)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
