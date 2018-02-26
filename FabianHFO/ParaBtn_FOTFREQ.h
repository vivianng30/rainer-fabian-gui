/**********************************************************************************************//**
 * \file	ParaBtn_FOTFREQ.h.
 *
 * Declares the para button fotfreq class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

// CParaBtn_FOTFREQ

class CParaBtn_FOTFREQ : public CParaBtn
{

public:
	explicit CParaBtn_FOTFREQ(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_FOTFREQ();

public:

	// Operations
public:
	virtual void Draw(int nState);




	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_FOTFREQ)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


