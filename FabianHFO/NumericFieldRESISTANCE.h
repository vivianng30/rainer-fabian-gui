#pragma once
#include "NumericField.h"

// CNumericFieldRESISTANCE

class CNumericFieldRESISTANCE : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldRESISTANCE)

public:
	CNumericFieldRESISTANCE(eNumericSize size);
	virtual ~CNumericFieldRESISTANCE();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldRESISTANCE)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


