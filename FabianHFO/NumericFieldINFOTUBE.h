#pragma once
#include "NumericField.h"

// CNumericFieldINFOTUBE

class CNumericFieldINFOTUBE : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldINFOTUBE)

public:
	CNumericFieldINFOTUBE(eNumericSize size);
	virtual ~CNumericFieldINFOTUBE();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldINFOTUBE)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

