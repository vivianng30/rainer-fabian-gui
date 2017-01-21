#pragma once
#include "parabtn.h"

class CParaBtn_PINSP :public CParaBtn
{
public:
	explicit CParaBtn_PINSP(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_PINSP();

public:

	// Operations
public:
	virtual void Draw(int nState);
	virtual void SetValue(fVALUE v, bool bRedraw=false);

protected:
	//{{AFX_MSG(CParaBtn_PINSP)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
