/**********************************************************************************************//**
 * \file	ParaBtn_FOTPMEANEND.h.
 *
 * Declares the para button fotpmeanend class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

// CParaBtn_FOTPMEANEND

class CParaBtn_FOTPMEANEND : public CParaBtn
{

public:
	explicit CParaBtn_FOTPMEANEND(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_FOTPMEANEND();

public:
	virtual void Draw(int nState);



	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_FOTPMEANEND)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


