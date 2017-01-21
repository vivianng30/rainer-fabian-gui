#pragma once
#include "NumericField.h"

// CNumericFieldSPO2

class CNumericFieldSPO2 : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldSPO2)

public:
	CNumericFieldSPO2(eNumericSize size);
	virtual ~CNumericFieldSPO2();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldSPO2)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


