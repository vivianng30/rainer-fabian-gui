#pragma once

#include "WndNumeric.h"

// CWndNumPSV_Av

class CWndNumPSV_Av : public CWndNumeric
{
	DECLARE_DYNAMIC(CWndNumPSV_Av)

public:
	CWndNumPSV_Av();
	virtual ~CWndNumPSV_Av();

	bool DrawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);



private:

	bool DrawFrames(CDC* pDC);
	bool DrawStaticText(CDC* pDC);
	bool DrawLimits(CDC* pDC);


protected:

	CString m_szName1;
	CString m_szName2;
	CString m_szName3;
	CString m_szName4;
	CString m_szName5;
	CString m_szName6;
	CString m_szName7;

	//CString m_szNameNote1;
	CString m_szNameNote2;
	CString m_szNameNote3;
	CString m_szNameNote4;

	CString m_szUnitPressure;
	CString m_szUnitLiter;
	CString m_szUnitMilliliter;
	CString m_szUnitBPM;
	CString m_szUnitPercent;

	//{{AFX_MSG(CWndNumPSV_Av)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


