#pragma once
#include "parabtn.h"

// CParaBtn_FOTPEEPSTART

class CParaBtn_FOTPEEPSTART : public CParaBtn
{

public:
	explicit CParaBtn_FOTPEEPSTART(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_FOTPEEPSTART();

public:
	virtual void Draw(int nState);

	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_FOTPEEPSTART)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

