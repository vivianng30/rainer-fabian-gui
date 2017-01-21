#pragma once
#include "NumericField.h"

// CNumericFieldSPO2PULSERATE

class CNumericFieldSPO2PULSERATE : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldSPO2PULSERATE)

public:
	CNumericFieldSPO2PULSERATE(eNumericSize size);
	virtual ~CNumericFieldSPO2PULSERATE();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldSPO2PULSERATE)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



