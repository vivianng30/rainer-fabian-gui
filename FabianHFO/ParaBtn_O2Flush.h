/**********************************************************************************************//**
 * \file	ParaBtn_O2Flush.h.
 *
 * Declares the para button o 2 flush class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

class CParaBtn_O2Flush :public CParaBtn
{
public:
	explicit CParaBtn_O2Flush(BTN &btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_O2Flush();
	//virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);

	void CheckValue(int iO2Value);

public:

	// Operations
public:
	virtual void Draw(int nState);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParaBtn_O2Flush)
public:
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_O2Flush)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};





