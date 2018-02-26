/**********************************************************************************************//**
 * \file	ParaBtn_PPSV.h.
 *
 * Declares the para button ppsv class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

class CViewParaBtn;

class CParaBtn_PPSV :public CParaBtn
{
public:
	explicit CParaBtn_PPSV(CViewParaBtn* parent, BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_PPSV();
	//virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);

	//void CheckValue(int iPEEPValue, bool bSEND);

public:

	// Operations
public:
	virtual void Draw(int nState);
	virtual void SetValue(fVALUE v, bool bRedraw=false);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParaBtn)
public:
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
protected:
	static CViewParaBtn* VIEWPARAPTN;


	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_PPSV)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
