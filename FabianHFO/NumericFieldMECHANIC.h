#pragma once
#include "NumericField.h"

// CNumericFieldMECHANIC

class CNumericFieldMECHANIC : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldMECHANIC)

public:
	CNumericFieldMECHANIC(eNumericSize size);
	virtual ~CNumericFieldMECHANIC();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	CString m_szName1;
	CString m_szName2;
	CString m_szName3;
	CString m_szName7;
	CString m_szName8;

	//CString m_szNameNote1;
	CString m_szNameNote2;

	CString m_szUnitMLMBAR;
	CString m_szUnitMBARLPS;
protected:
	//{{AFX_MSG(CNumericFieldMECHANIC)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

