#pragma once

#include "WndNumeric.h"

// CWndNumSIMVPSV_Mech

class CWndNumSIMVPSV_Mech : public CWndNumeric
{
	DECLARE_DYNAMIC(CWndNumSIMVPSV_Mech)

public:
	CWndNumSIMVPSV_Mech();
	virtual ~CWndNumSIMVPSV_Mech();

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
	CString m_szName8;
	CString m_szName9;
	CString m_szName10;
	CString m_szName11;

	//CString m_szNameNote1;
	CString m_szNameNote2;
	CString m_szNameNote3;
	CString m_szNameNote4;
	CString m_szNameNote5;
	CString m_szNameNote6;

	CString m_szUnitMLMBAR;
	CString m_szUnitMBARLPS;
	CString m_szUnitLiter;
	CString m_szUnitMilliliter;
	CString m_szUnitBPM;
	CString m_szUnitPercent;

	//{{AFX_MSG(CWndNumSIMVPSV_Mech)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
