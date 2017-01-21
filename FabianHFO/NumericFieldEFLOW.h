#pragma once
#include "NumericField.h"

// CNumericFieldEFLOW

class CNumericFieldEFLOW : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldEFLOW)

public:
	CNumericFieldEFLOW(eNumericSize size);
	virtual ~CNumericFieldEFLOW();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldEFLOW)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



