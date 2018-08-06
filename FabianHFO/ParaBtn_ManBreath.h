/**********************************************************************************************//**
 * \file	ParaBtn_ManBreath.h.
 *
 * Declares the para button manager breath class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

// CParaBtn_ManBreath

class CParaBtn_ManBreath : public CParaBtn
{
	DECLARE_DYNAMIC(CParaBtn_ManBreath)

public:
	explicit CParaBtn_ManBreath(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_ManBreath();

	virtual void Draw(int nState);
protected:
	//{{AFX_MSG(CParaBtn_ManBreath)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


