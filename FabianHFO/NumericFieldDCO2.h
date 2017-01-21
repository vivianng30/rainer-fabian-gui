#pragma once
#include "NumericField.h"

// CNumericFieldDCO2

class CNumericFieldDCO2 : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldDCO2)

public:
	CNumericFieldDCO2(eNumericSize size);
	virtual ~CNumericFieldDCO2();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldDCO2)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


