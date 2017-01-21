#pragma once
#include "parabtn.h"

class CViewParaBtn;

// CParaBtn_PMEAN_REC

class CParaBtn_PMEAN_REC : public CParaBtn
{
	DECLARE_DYNAMIC(CParaBtn_PMEAN_REC)

public:
	explicit CParaBtn_PMEAN_REC(CViewParaBtn* parent, BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_PMEAN_REC();

	virtual void Draw(int nState);
protected:
	static CViewParaBtn* VIEWPARAPTN;
	//void CheckValue(int iValue, bool bSend);
protected:
	//{{AFX_MSG(CParaBtn_PMEAN_REC)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


