#pragma once
#include "NumericField.h"

// CNumericFieldPPEAK

class CNumericFieldPPEAK : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldPPEAK)

public:
	CNumericFieldPPEAK(eNumericSize size);
	virtual ~CNumericFieldPPEAK();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldPPEAK)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
