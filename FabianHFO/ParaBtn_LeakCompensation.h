/**********************************************************************************************//**
 * \file	ParaBtn_LeakCompensation.h.
 *
 * Declares the para button leak compensation class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

// CParaBtn_LeakCompensation

class CParaBtn_LeakCompensation : public CParaBtn
{
	DECLARE_DYNAMIC(CParaBtn_LeakCompensation)

public:
	explicit CParaBtn_LeakCompensation(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_LeakCompensation();
	virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);

	virtual void Draw(int nState);
protected:


	//{{AFX_MSG(CParaBtn_LeakCompensation)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


