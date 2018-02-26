/**********************************************************************************************//**
 * \file	ParaBtn_AbortPSV.h.
 *
 * Declares the para button abort psv class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

// CParaBtn_AbortPSV

class CParaBtn_AbortPSV : public CParaBtn
{
	DECLARE_DYNAMIC(CParaBtn_AbortPSV)

public:
	explicit CParaBtn_AbortPSV(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_AbortPSV();

	virtual void Draw(int nState);
protected:
	//{{AFX_MSG(CParaBtn_BACKUP)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


