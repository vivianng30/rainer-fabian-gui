/**********************************************************************************************//**
 * \file	ParaBtn_PCurve.h.
 *
 * Declares the para button p curve class
 **************************************************************************************************/

#pragma once
#include "parabtn.h"

// CParaBtn_PCurve

class CParaBtn_PCurve : public CParaBtn
{
	DECLARE_DYNAMIC(CParaBtn_PCurve)

public:
	explicit CParaBtn_PCurve(BTN btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn_PCurve();
	virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);

	virtual void Draw(int nState);
protected:

	CBmp* m_pcIFlow;
	CBmp* m_pcLinear;
	CBmp* m_pcAutoflow;

	//{{AFX_MSG(CParaBtn_BACKUP)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


