#pragma once
#include "NumericField.h"

// CNumericFieldFREQETCO2

class CNumericFieldFREQETCO2 : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldFREQETCO2)

public:
	CNumericFieldFREQETCO2(eNumericSize size);
	virtual ~CNumericFieldFREQETCO2();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldFREQETCO2)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



