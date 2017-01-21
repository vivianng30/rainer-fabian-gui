#pragma once
#include "NumericField.h"

// CNumericFieldSPO2PI

class CNumericFieldSPO2PI : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldSPO2PI)

public:
	CNumericFieldSPO2PI(eNumericSize size);
	virtual ~CNumericFieldSPO2PI();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFiCNumericFieldSPO2PIeldSize1)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};




