#pragma once
#include "NumericField.h"

// CNumericFieldFICO2

class CNumericFieldFICO2 : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldFICO2)

public:
	CNumericFieldFICO2(eNumericSize size);
	virtual ~CNumericFieldFICO2();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldFICO2)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
