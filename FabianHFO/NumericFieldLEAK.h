#pragma once
#include "NumericField.h"

// CNumericFieldLEAK

class CNumericFieldLEAK : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldLEAK)

public:
	CNumericFieldLEAK(eNumericSize size);
	virtual ~CNumericFieldLEAK();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldLEAK)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

