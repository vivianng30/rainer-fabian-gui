#pragma once
#include "NumericField.h"

// CNumericFieldINFOPATAL

class CNumericFieldINFOPATAL : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldINFOPATAL)

public:
	CNumericFieldINFOPATAL(eNumericSize size);
	virtual ~CNumericFieldINFOPATAL();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);
protected:
	CString m_szName1;
	CString m_szName2;

protected:
	//{{AFX_MSG(CNumericFieldINFOPATAL)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

