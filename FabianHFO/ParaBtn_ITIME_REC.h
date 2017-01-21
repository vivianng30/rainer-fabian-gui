#pragma once
#include "parabtn.h"

// CParaBtn_ITIME_REC

class CParaBtn_ITIME_REC : public CParaBtn
{
	DECLARE_DYNAMIC(CParaBtn_ITIME_REC)

public:
	explicit CParaBtn_ITIME_REC(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_ITIME_REC();

	virtual void Draw(int nState);
protected:
	//{{AFX_MSG(CParaBtn_ITIME_REC)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


