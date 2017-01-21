#pragma once
#include "parabtn.h"

class CViewParaBtn;

class CParaBtn_PMITT :public CParaBtn
{
public:
	explicit CParaBtn_PMITT(CViewParaBtn* parent, BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_PMITT();

public:
	virtual void Draw(int nState);
protected:
	static CViewParaBtn* VIEWPARAPTN;
	

	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_PMITT)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
