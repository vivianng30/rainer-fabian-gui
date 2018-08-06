/**********************************************************************************************//**
 * \file	ParaBtn_BPM.h.
 *
 * Declares the para button bpm class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

class CParaBtn_BPM :public CParaBtn
{
public:
	explicit CParaBtn_BPM(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_BPM();

public:

	// Operations
public:
	virtual void Draw(int nState);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDParaBtn)
public:
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL



	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_BPM)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



