#pragma once
#include "parabtn.h"

// CParaBtn_RISETIME

class CParaBtn_RISETIME : public CParaBtn
{
	DECLARE_DYNAMIC(CParaBtn_RISETIME)

public:
	explicit CParaBtn_RISETIME(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_RISETIME();

	virtual void Draw(int nState);


protected:
	//{{AFX_MSG(CParaBtn_RISETIME)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

