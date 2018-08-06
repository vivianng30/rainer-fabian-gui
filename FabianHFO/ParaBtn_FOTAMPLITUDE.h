/**********************************************************************************************//**
 * \file	ParaBtn_FOTAMPLITUDE.h.
 *
 * Declares the para button fotamplitude class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

// CParaBtn_FOTAMPLITUDE

class CParaBtn_FOTAMPLITUDE : public CParaBtn
{

public:
	explicit CParaBtn_FOTAMPLITUDE(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_FOTAMPLITUDE();

public:

	// Operations
public:
	virtual void Draw(int nState);





	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_FOTAMPLITUDE)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


