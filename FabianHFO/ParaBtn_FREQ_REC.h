#pragma once
#include "parabtn.h"

// CParaBtn_FREQ_REC

class CParaBtn_FREQ_REC : public CParaBtn
{
	DECLARE_DYNAMIC(CParaBtn_FREQ_REC)

public:
	explicit CParaBtn_FREQ_REC(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_FREQ_REC();

	virtual void Draw(int nState);
protected:
	//{{AFX_MSG(CParaBtn_FREQ_REC)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


