#pragma once
#include "parabtn.h"

// CParaBtn_FOTSTEPS

class CParaBtn_FOTSTEPS : public CParaBtn
{

public:
	explicit CParaBtn_FOTSTEPS(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_FOTSTEPS();

public:

	// Operations
public:
	virtual void Draw(int nState);





	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn_FOTSTEPS)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


