#pragma once
#include "parabtn.h"

// CParaBtn_FOTPMEANSTART

class CParaBtn_FOTPMEANSTART : public CParaBtn
{
public:
	explicit CParaBtn_FOTPMEANSTART(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_FOTPMEANSTART();

public:
	virtual void Draw(int nState);

	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_FOTPMEANSTART)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


