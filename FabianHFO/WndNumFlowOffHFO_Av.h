#pragma once

#include "WndNumeric.h"

class CWndNumFlowOffHFO_Av : public CWndNumeric
{
	DECLARE_DYNAMIC(CWndNumFlowOffHFO_Av)

public:
	CWndNumFlowOffHFO_Av();
	virtual ~CWndNumFlowOffHFO_Av();

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
	
	//CString m_szNameNote1;
	CString m_szNameNote2;
	CString m_szNameNote3;

	CString m_szUnitPressure;

	//{{AFX_MSG(CWndNumFlowOffHFO_Av)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};





