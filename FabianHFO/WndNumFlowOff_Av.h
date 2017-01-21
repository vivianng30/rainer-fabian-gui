#pragma once

#include "WndNumeric.h"

class CWndNumFlowOff_Av : public CWndNumeric
{
	DECLARE_DYNAMIC(CWndNumFlowOff_Av)

public:
	CWndNumFlowOff_Av();
	virtual ~CWndNumFlowOff_Av();

	bool DrawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);

private:

	bool DrawFrames(CDC* pDC);
	bool DrawStaticText(CDC* pDC);
	bool DrawLimits(CDC* pDC);
	

protected:

	bool m_bCPAP;

	CString m_szName1;
	CString m_szName2;
	CString m_szName3;
	CString m_szName4;
	CString m_szName5;
	CString m_szName6;

	//CString m_szNameNote1;
	CString m_szNameNote2;

	CString m_szUnitPressure;
	CString m_szUnitBPM;

	//{{AFX_MSG(CWndNumFlowOff_Av)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};





