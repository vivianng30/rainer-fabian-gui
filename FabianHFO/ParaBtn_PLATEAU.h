#pragma once
#include "parabtn.h"

// CParaBtn_PLATEAU

class CParaBtn_PLATEAU : public CParaBtn
{
	DECLARE_DYNAMIC(CParaBtn_PLATEAU)

public:
	explicit CParaBtn_PLATEAU(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_PLATEAU();

	virtual void Draw(int nState);


protected:
	//{{AFX_MSG(CParaBtn_PLATEAU)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


