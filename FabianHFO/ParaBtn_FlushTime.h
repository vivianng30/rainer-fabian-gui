#pragma once
#include "parabtn.h"

// CParaBtn_FlushTime

class CParaBtn_FlushTime : public CParaBtn
{
	DECLARE_DYNAMIC(CParaBtn_FlushTime)

public:
	explicit CParaBtn_FlushTime(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_FlushTime();

	virtual void Draw(int nState);
protected:
	//{{AFX_MSG(CParaBtn_FlushTime)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


