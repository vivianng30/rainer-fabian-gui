#pragma once
#include "NumericField.h"

// CNumericFieldPMEAN

class CNumericFieldPMEAN : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldPMEAN)

public:
	CNumericFieldPMEAN(eNumericSize size);
	virtual ~CNumericFieldPMEAN();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldPMEAN)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
