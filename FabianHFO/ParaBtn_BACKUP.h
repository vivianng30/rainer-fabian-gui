/**********************************************************************************************//**
 * \file	ParaBtn_BACKUP.h.
 *
 * Declares the para button backup class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

class CParaBtn_BACKUP :public CParaBtn
{
public:
	explicit CParaBtn_BACKUP(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_BACKUP();
	//virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);


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
	//{{AFX_MSG(CParaBtn_BACKUP)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};






