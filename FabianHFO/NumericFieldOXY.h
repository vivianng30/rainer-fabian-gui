#pragma once
#include "NumericField.h"

// CNumericFieldOXY

class CNumericFieldOXY : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldOXY)

public:
	CNumericFieldOXY(eNumericSize size);
	virtual ~CNumericFieldOXY();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldOXY)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



