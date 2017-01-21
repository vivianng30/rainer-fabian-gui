#pragma once
#include "NumericField.h"

// CNumericFieldPEEP

class CNumericFieldPEEP : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldPEEP)

public:
	CNumericFieldPEEP(eNumericSize size);
	virtual ~CNumericFieldPEEP();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldPEEP)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

