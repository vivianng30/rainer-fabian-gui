#pragma once
#include "parabtn.h"


// CParaBtn_FOTPEEPEND

class CParaBtn_FOTPEEPEND : public CParaBtn
{

public:
	explicit CParaBtn_FOTPEEPEND(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_FOTPEEPEND();
public:
	virtual void Draw(int nState);

protected:
	//{{AFX_MSG(CParaBtn_FOTPEEPEND)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


