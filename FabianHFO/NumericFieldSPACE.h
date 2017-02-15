#pragma once
#include "NumericField.h"

// CNumericFieldSPACE

class CNumericFieldSPACE : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldSPACE)

public:
	CNumericFieldSPACE(eNumericSize size);
	virtual ~CNumericFieldSPACE();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldSPACE)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

