/**********************************************************************************************//**
 * \file	ParaBtn_TRIGGER.h.
 *
 * Declares the para button trigger class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

class CParaBtn_TRIGGER :public CParaBtn
{
public:
	CParaBtn_TRIGGER(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_TRIGGER();
	//virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);


public:

	void Trigger();

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
	//{{AFX_MSG(CParaBtn_TRIGGER)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};





