#pragma once
#include "NumericField.h"

// CNumericFieldPSVTIME

class CNumericFieldPSVTIME : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldPSVTIME)

public:
	CNumericFieldPSVTIME(eNumericSize size);
	virtual ~CNumericFieldPSVTIME();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldPSVTIME)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

